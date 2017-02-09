#!/bin/bash

if [ -z $1 ] ; then
  PREFIX="/usr"
else
  PREFIX="$1"
fi

if [ -z $LIBDIR ] ; then
  LIBDIR="$PREFIX/lib"
fi

if [ ! -w $LIBDIR ] ; then
  echo "You do not have write permissions for $LIBDIR!"
  echo "Make sure you have the correct permissions or supply a prefix with:"
  echo "  $0 /your/prefix"
  exit 1
fi

(cd winexe; ln -fs libwincmd.so.0.80 libwincmd.so; cd ..)
cp -P winexe/libwincmd.so* $LIBDIR

if [ ! -d $LIBDIR/pkgconfig ] ; then
  mkdir -p $LIBDIR/pkgconfig
fi

PCFILE="$LIBDIR/pkgconfig/wincmd.pc"

echo "prefix=$PREFIX" > $PCFILE
echo "exec_prefix=$PREFIX" >> $PCFILE
echo "libdir=$LIBDIR" >> $PCFILE
echo "includedir=$PREFIX/include" >> $PCFILE
echo "" >> $PCFILE
echo "Name: wincmd" >> $PCFILE
echo "Description: wincmd library for OpenVAS" >> $PCFILE
echo "Version: 0.80" >> $PCFILE
echo "Requires:" >> $PCFILE
echo "Cflags: -I\${includedir} -I\${includedir}/openvas" >> $PCFILE
echo "Libs: -L\${libdir}" >> $PCFILE

