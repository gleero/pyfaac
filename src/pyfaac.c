#include <Python.h>
#include <faac.h>


unsigned long sampleRate = 44100;
unsigned int numChannels = 2;
unsigned int bitRate = 64000;
unsigned int nPCMBitSize = 16;
int bufSize;
int samples = 0;
unsigned long inputSamples;
unsigned long maxOutputBytes;
faacEncHandle hEncoder;
faacEncConfigurationPtr faacConfig;


// Конструктор питон-класса
static PyObject* pyfaac_init(PyObject *self, PyObject *args)
{
    PyObject* obj;

    // Вытаскиваем параметры из параметров конструктора
    if (!PyArg_ParseTuple(args, "OkII", &obj, &sampleRate, &numChannels, &bitRate)) {
        return NULL;
    }

    // Создаём енкодер
    hEncoder = faacEncOpen(sampleRate, numChannels, &inputSamples, &maxOutputBytes);
    if (!hEncoder) {
         PyErr_SetString(PyExc_Exception, "Can not open Faac encoder");
        return NULL;
    }

    // Конфигурация енкодера
    faacConfig = faacEncGetCurrentConfiguration(hEncoder);
    faacConfig->aacObjectType = LOW;
    faacConfig->inputFormat = FAAC_INPUT_FLOAT;
    faacConfig->mpegVersion = MPEG2;
    faacConfig->outputFormat = 1;
    faacConfig->allowMidside = 1;
    faacConfig->shortctl = 0;
    faacConfig->useTns = 0;
    faacConfig->bitRate = bitRate;

    if (!faacEncSetConfiguration(hEncoder, faacConfig)) {
        PyErr_SetString(PyExc_Exception, "Can not apply Faac configuration");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

// Кодируем и свопаем байты
static PyObject* pyfaac_encode(PyObject *self, PyObject *args)
{

    char *char_buffer;
    PyObject* obj;
    int size;

    if (!PyArg_ParseTuple(args, "Os#", &obj, &char_buffer, &size)) {
        return NULL;
    }

    float *pcmbuf = (float *)malloc(inputSamples*sizeof(float));
    unsigned char *bitbuf = (unsigned char*)malloc(maxOutputBytes*sizeof(unsigned char));

    size_t i = 0;
    int s;
    int bytesWritten;
    unsigned char bufi[2];
    char *current_ptr = char_buffer;

    while (i < inputSamples) {
        memcpy(bufi, current_ptr, 2);
        s = ((int16_t *)bufi)[0];
        pcmbuf[i] = (float)s;
        current_ptr += 2;
        i++;
    }

    bytesWritten = faacEncEncode(hEncoder, (int32_t *)pcmbuf, i, bitbuf, maxOutputBytes);
    // printf("bytesWritten = %d\n", bytesWritten);

    PyObject* tmpstr;
    tmpstr = Py_BuildValue("s#", bitbuf, bytesWritten);
    return tmpstr;
}

static PyObject* pyfaac_getSize(PyObject *self, PyObject *args)
{

    PyObject* bsize;
    bsize = Py_BuildValue("I", inputSamples * 2);
    return bsize;
}

static PyObject* pyfaac_close(PyObject *self, PyObject *args)
{
    // printf("samples = %d \n", samples);

    PyObject* tmpstr;

    unsigned char data[maxOutputBytes];

    while(bufSize != 0){
        bufSize = faacEncEncode(hEncoder, NULL, 0, data, maxOutputBytes);
        // printf("---- buffer size = %d \n", bufSize);
        tmpstr = Py_BuildValue("s#", data, bufSize);
    }

    faacEncClose(hEncoder);
    return tmpstr;
}

static PyMethodDef pyfaacMethods[] =
{
    {"__init__", pyfaac_init, METH_VARARGS,
     "doc string"},
    {"encode", pyfaac_encode, METH_VARARGS,
     "doc string"},
     {"close", pyfaac_close, METH_VARARGS,
     "doc string"},
     {"getSize", pyfaac_getSize, METH_VARARGS,
     "doc string"},
    {NULL},
};

static PyMethodDef ModuleMethods[] = { {NULL} };

#ifdef __cplusplus
extern "C"
#endif

PyMODINIT_FUNC initpyfaac(void)
{
    PyMethodDef *def;

    PyObject *module = Py_InitModule("pyfaac", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("pyfaac");
    PyObject *pyfaacClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "pyfaac", pyfaacClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(pyfaacClass);

    for (def = pyfaacMethods; def->ml_name != NULL; def++) {
        PyObject *func = PyCFunction_New(def, NULL);
        PyObject *method = PyMethod_New(func, NULL, pyfaacClass);
        PyDict_SetItemString(classDict, def->ml_name, method);
        Py_DECREF(func);
        Py_DECREF(method);
    }
}

