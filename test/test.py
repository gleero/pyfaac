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
