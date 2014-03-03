Run MapTest2.m
which calls YonderWay.m (Initially Called MapHacks.m)

The parameters you can change quickly are 

# Static Map Method 3

Accomplished using Moore-Penrose Pseudoinverses
```python
mean_lat = average(samples.lat)
mean_lon = average(samples.lon)
xs = (samples.lat - mean_lat)
ys = (samples.lon - mean_lon)
    
A = [xs.^2 | ys.^2 | xs | ys | 1]
Ac = c_transpose(A)
   
coeff_vec = ((A*Ac)^-1)*Ac*(samples.rssi)
    
for points in game_map:
    lat =  point.lat - mean_lat
    lon = point.lon - mean_lon
    point.rssi = [ lat^2 lon^2 lat lon 1] * coeff_vec
    
post_process(game_map)
        
    
