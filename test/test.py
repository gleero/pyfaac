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
