from distutils.core import setup, Extension

pyfaac = Extension('pyfaac', sources = ['src/pyfaac.c'], libraries=["faac"])

setup (
    name = 'PyFaac',
    version = '1.0',
    description = 'A fast sound encoder',
    ext_modules = [pyfaac]
)
