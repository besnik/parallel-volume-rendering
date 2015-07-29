Parallel Volume Rendering
=========================

In this work we explore and discuss visualization of large volume data on parallel architectures. In the paper we focus a bit on the data from confocal microscopy. Note that described techniques, algorithms and optimalizations are not coupled to the confocal data. All approaches can be applied on any volumetric data - does not matter if it is from microscope, sonar, geological data, medical data like MRI or CT; or for example syntetic physic data.

#### Read [document here](parallel-volume-rendering/wiki).

Note: the document is still being translated from Slovak version that you can download in the downloads section or at the bottom of this file.

Abstract
--------
The thesis provides a presentation of a parallel volume data visualization system for data
supplied from confocal laser scanning microscopy. It describes and compares various methods
for volume visualization and optimalization and their implementations on parallel architectures.
The parallel volume renderer we have developed is implemented on a 16 nodes PC cluster
connected by a high-speed Myrinet network using MPI, ray-casting and binary-swap algorithm.
To improve efficiency, several optimalizations for increasing the speed of the ray casting process
are used, including empty regions skipping and an efficient bricking addressing scheme.
We also suggest several methods of improving the performance when using MPI and compilers.
The system is capable of running interactively (several frames per second) on a PC cluster.
Keywords: Volume ray-casting, parallel rendering, PC clusters, binary swap algorithm.

Sources
-------
Source codes can be found in sources directory. Many comments and method names are unfortunately in slovak languages. I will try to highlight important parts, algorithms and optimalizations in the wiki. After those years when looking back in the sources it looks horrible :-) No separation of concerns, no unit tests, many things mixed up. But with that time frame, knowledge (nobody knew parallel programming and I had to study everything alone), complications with cluster and remote development I'm still quite proud what was achieved. When I was discussing the work with people in laser centre, they said I was kind of pioneer in this field of parallel rendering of volume data in Slovakia :)

Documentation
-------------
Presentations and documentation for some algorithms are located in [download](/besnik/parallel-volume-rendering/downloads) section. Most of them are in Slovak language. I'll try to translate them into the English when I find some time.

 * [Visualization of Data from Confocal Microscopy](/downloads/besnik/parallel-volume-rendering/pvr-presentation-18.1.2006.ppt)
 * [MPI - Message Passing Interface](/downloads/besnik/parallel-volume-rendering/MPI-Message_Passing_Interface_en.ppt)

Images
------
Please see [Images](/besnik/parallel-volume-rendering/tree/master/images) directory. Also some images can be found in the thesis, pdf or powerpoint presentations in [download](/besnik/parallel-volume-rendering/downloads) section.

![Comparison of various threshold values by bezier curve editor in the client](https://raw.githubusercontent.com/besnik/parallel-volume-rendering/master/images/comparison-of-bezier-threshold-biological-data.jpg)

![Example of syntetic data cube-in-cube. High versus Low threshold value.](https://raw.githubusercontent.com/besnik/parallel-volume-rendering/master/images/no-threshold-vs-threshold.jpg)

Thesis
------
During the work we have checked many science articles; many from the world leaders in the field of volume visualization. Some articles about especially complex optimalizations contained formulas with a mistake in it. In the sample source codes you can find correct versions of the algorithms implemented in C++ language.

The thesis is great introduction to vizualization itself and give reader a bit inside how complex is to handle massive volume data for example in medicine area. On the other hand - with improvements in algorithms and hardware - it shows that it is not impossible to implement and use tools in hospitals like we can see currently only in scifi movies.

Last but not least - in general the parallel techniques for handling massive amount of data is not coupled only to the graphics field. The same principles like *Divide and Conquer* applies to all other areas. Same principles can be applied when handling massive data in the databases. These days rise of NoSql solutions like Mongo, CouchDB or Cassandra makes it possible to build very scalable business systems. There are many similarities between those big systems like twitter or facebook, and parallel rendering of large amount of data. It can be interesting to look at the problem from this perspective.

Slovak version
--------------
Note for Czechoslovakian people - there is also PDF version available in [Slovak language](/downloads/besnik/parallel-volume-rendering/vizualizacia_dat_z_konfokalneho_mikroskopu-hudak_slavomir.pdf).
