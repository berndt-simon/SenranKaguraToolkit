# SenranKaguraToolkit

Some sort of Middleware to work with Files from the PC Version of 'Senran Kagura - Shinovi Versus'.  

For Informations regarding the File-Formats see the [Wiki](https://github.com/berndt-simon/SenranKaguraToolkit/wiki)

The Middleware itself is in a _very very_ early Stage.

Things that work at the Moment:
* Generate a List of all Files from filename.bin
* Read Content of CAT-Files
* Read Content of GXT-Files (standalone or within a CAT-File)
* Dump GXT-Content as DDS-Files - replicating internal File-Structure with new Folders
* Read Content of TMD-Files (standalone or within a CAT-File)
* Dump TMD-Content as OBJ-Files - sort of works (at the Moment just basic Geometry and UV-Map)

I dared to require Boost::Filesystem because working without it would be a mess..
