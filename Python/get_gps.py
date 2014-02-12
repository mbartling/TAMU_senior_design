#cs = current state
print 'start script'

while cs.lat == 0:
	print 'Waiting for GPS'
	Script.Sleep(1000)

while i < 1000:
	Script.Sleep(500)
	cs.lng
	cs.lat
	i = i+1
