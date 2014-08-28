# pyfaac

A easy python module taking raw PCM audio to ADTS AAC using libfaac.

## Install

You need installed packages:

- `libfaac-dev`
- `python-dev`

and c compiler.


Build:

`python setup.py install`


## Using

```python
import pyfaac

samplerate = 16000
channels = 1
bitrate = 92000

input = open('in.wav', 'rb')
output = open('out.aac', 'wb')

codec = pyfaac.pyfaac(samplerate, channels, bitrate)
size = obj.getSize()

while True:
    data = input.read(size)
    if data:
        stream = obj.encode(data)
        output.write(stream)
    else:
        break

excess = obj.close()

input.close()
output.write(excess)
output.flush()
output.close()
```

## Interface

```
pyfaac.pyfaac(samplerate, channels, bitrate)
```
- `samplerate` — source PCM sample rate. AAC requires a minimum 16000 KHz;
- `channels` — source PCM channels. 1 — mono, 2 — stereo;
- `bitrate` — output AAC bitrate.


```
obj.getSize()
```
Returns the maximum size of the data to be transmitted to the encoder.

```
aac = obj.encode(data)
```

Encode raw PCM data to AAC. Each fragment begins with ADTS header and ready for transport stream.

```
aac = obj.close()
```

Close encoder and returns the residual AAC data.