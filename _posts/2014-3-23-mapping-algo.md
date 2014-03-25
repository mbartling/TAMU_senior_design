---
layout: post
title: Example Static Mapping Algorithm
---

## Table of Contents
- Overview
- Static Mapping: Moore-Penrose Pseudoinverse
- References

## Overview

## Static Mapping: Moore-Penrose Pseudoinverse

```python
mean_lat = average(samples.lat) 
mean_lon = average(samples.lon) 

xs = (samples.lat - mean_lat) 
ys = (samples.lon - mean_lon) 

A = [xs.^2 | ys.^2 | xs*ys | xs | ys | 1] 
Ac = c_transpose(A) 

coeff_vec = ((A*Ac)^-1)*Ac*(samples.rssi) 

for	points in game_map: 
	lat = point.lat - mean_lat 
	lon = point.lon - mean_lon 
	point.rssi = [ lat^2 lon^2 lat lon 1] * coeff_vec

post_process(game_map)
```

## References
