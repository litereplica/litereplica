litereplica
===========

SQLite replication made easy

The litereplica allows your applications to easily keep replicas of SQLite3 databases

For a complete list of features, visit our site http://litereplica.io


Requirements
------------

The litereplica depends on the [binn](https://github.com/liteserver/binn) and the [nanomsg](https://github.com/nanomsg/nanomsg) libraries


How to use
----------

You can include the `sqlite3.c` file in your applications or leave it as a separate shared library

Them same apply for the `binn` and the `nanomsg` libraries


Linking to the shared library
-----------------------------

##### On Linux:
```
gcc -lsqlite3 myapp.c
```

##### On Windows:

Include the `sqlite3.lib` in your MSVC project


Compiling the Library
---------------------

##### On Linux:

```
git clone https://github.com/litereplica/litereplica
cd litereplica
make
make install
```
It will create the `libsqlite3.so.1` library and the `sqlite3` shell application


##### On Windows:

Use the included Visual Studio project in the win32 folder

It will create the library `sqlite3-1.0.dll` but you can rename it to `litereplica-1.0.dll`


Licencing
---------

If your application includes or links to the litereplica code you will need a license.

The litereplica code is released under one of these 2 licenses:

 * GNU Affero GPL v3
 * Commercial License

Use the first option if your applications are released under a license compatible with the GNU Affero GPL v3. This means also releasing the source code.

Otherwise you can purchase a commercial license at http://litereplica.io
