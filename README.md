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
size = codec.getSize()

while True:
    data = input.read(size)
    if data:
        stream = codec.encode(data)
        output.write(stream)
    else:
        break

excess = codec.close()

input.close()
output.write(excess)
output.flush()
output.close()
```

## Interface

```
codec = pyfaac.pyfaac(samplerate, channels, bitrate)
```

- `samplerate` — source PCM sample rate. AAC requires a minimum 16000 KHz;
- `channels` — source PCM channels. 1 — mono, 2 — stereo;
- `bitrate` — output AAC bitrate.

```
codec.getSize()
```

Returns the maximum size of data to be transmitted to the encoder.

```
aac = codec.encode(data)
```

Encode raw PCM data to AAC. Each fragment begins with ADTS header and ready for transport stream.

```
aac = codec.close()
```

Close encoder and returns the residual AAC data.