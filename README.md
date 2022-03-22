# CGProjectpub
note that RAM usage seems rather high with pandas and pickle? with 16GB of RAM
and 5000 passing cells from LV1, LV2 uses nearly all of it
it would be much better to make lv2 and 3 into a generator function then 
call slices from pandas and accumulate the results, then we wouldn't worry
about RAM limitations.