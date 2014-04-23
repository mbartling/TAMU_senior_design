from sets import Set

lon = 0xc693a9bb

lon = lon ^ 0xffffffff
print lon

lon += 1
print lon

good_ant_addresses = Set([101])
bad_ant_addresses = Set([103])

ant_addresses = good_ant_addresses.union(bad_ant_addresses)

print ant_addresses


