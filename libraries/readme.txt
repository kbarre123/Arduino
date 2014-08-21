Notes on Libraries:

* To use the Seeedstudio 2.8" touchscreen, I had to download the Adafruit library
    entitled "TouchScreen". However, the examples contained therein are for the 
    v2 version of my shield. So, I also had to download the TouchShield version
    from the Seeedstudion Github. Those sketches wor with my version, but a few
    changes need to be made to them, as the Adafruit library was updated. One 
    example is, in the 'paint' example sketch, Point needs to be changed to TSPoint.
