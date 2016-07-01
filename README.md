# SenranKaguraToolkit

Some sort of Middleware to work with files from the PC Version of 'Senran Kagura - Shinovi Versus'.  

For Informations regarding the File-Formats see the [Wiki](https://github.com/berndt-simon/SenranKaguraToolkit/wiki)

The Middleware itself is in a very _very_ early Stage.

Things that work at the Moment:
* Generate a List of all Files from filename.bin
* Inspect Contend of CAT-Files
* Extract Contend of GXT-Files (standalone or within a CAT-File)
* Dump GXT-Content as DDS-Files - replicating internal File-Structure with new Folders

I dared to require Boost::Filesystem because working without it is a mess..
