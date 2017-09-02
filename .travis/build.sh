#!/bin/sh

#
# actual build script
# most of the steps are ported from the aranym.spec file
#
TMP="${PWD}/.travis/tmp"
OUT="${PWD}/.travis/out"

mkdir -p "${TMP}"
mkdir -p "${OUT}"

unset CC CXX

prefix=/usr
bindir=$prefix/bin
datadir=$prefix/share
icondir=$datadir/icons/hicolor

if test "$suse_version" -ge 1200; then
	with_nfosmesa=--enable-nfosmesa
fi
common_opts="--prefix=$prefix --enable-addressing=direct --enable-usbhost --disable-sdl2 $with_nfosmesa"

VERSION=`sed -n -e 's/#define.*VER_MAJOR.*\([0-9][0-9]*\).*$/\1./p
s/#define.*VER_MINOR.*\([0-9][0.9]*\).*$/\1./p
s/#define.*VER_MICRO.*\([0-9][0-9]*\).*$/\1/p' src/include/version.h | tr -d '\n'`

NO_CONFIGURE=1 ./autogen.sh

isrelease=false
ATAG=${VERSION}${archive_tag}
tag=`git tag --points-at ${TRAVIS_COMMIT}`
case $tag in
	ARANYM_*)
		isrelease=true
		;;
	*)
		ATAG=${VERSION}${archive_tag}-${SHORT_ID}
		;;
esac

case $CPU in
	i[3456]86 | x86_64 | arm*) build_jit=true ;;
	*) build_jit=false ;;
esac

case "$TRAVIS_OS_NAME" in
linux)
	if $build_jit; then
	mkdir jit
	cd jit
	../configure $common_opts --enable-jit-compiler --enable-jit-fpu || exit 1
	make depend
	make || exit 1
	cd ..
	fi
	
	mkdir mmu
	cd mmu
	../configure $common_opts --enable-lilo --enable-fullmmu || exit 1
	make depend
	make || exit 1
	cd ..
	
	./configure $common_opts || exit 1
	make depend
	make || exit 1
	
	make DESTDIR="$TMP" install-strip || exit 1
	sudo chown root "$TMP${bindir}/aratapif"
	sudo chgrp root "$TMP${bindir}/aratapif"
	sudo chmod 4755 "$TMP${bindir}/aratapif"
	if $build_jit; then
	install -s -m 755 jit/src/aranym "$TMP${bindir}/aranym-jit"
	fi
	install -s -m 755 mmu/src/aranym "$TMP${bindir}/aranym-mmu"
	for s in 32 48; do
	  install -d "$TMP${icondir}${s}x${s}/apps/"
	  install -m 644 contrib/icon-$s.png "$TMP${icondir}${s}x${s}/apps/aranym.png"
	done
	
	install -d "$TMP${datadir}/applications"
	for name in aranym aranym-jit aranym-mmu; do
		install -m 644 contrib/$name.desktop "$TMP/${datadir}/applications/$name.desktop"
	done

	ARCHIVE="${PROJECT_LOWER}-${ATAG}.tar.xz"
	(
	cd "${TMP}"
	tar cvfJ "${OUT}/${ARCHIVE}" .
	)
	;;

osx)
	DMG="${PROJECT_LOWER}-${VERSION}${archive_tag}.dmg"
	ARCHIVE="${PROJECT_LOWER}-${ATAG}.dmg"
	(
	cd src/Unix/MacOSX
	xcodebuild -derivedDataPath "$OUT" -project MacAranym.xcodeproj -configuration Release -scheme Packaging 
	)
	mv "$OUT/Build/Products/Release/$DMG" "$OUT/$ARCHIVE" || exit 1
	;;

esac

export ARCHIVE
export isrelease

if $isrelease; then
	make dist
	for ext in gz bz2 xz lz; do
		ARCHIVE="${PROJECT_LOWER}-${VERSION}.tar.${ext}"
		test -f "${ARCHIVE}" && mv "${ARCHIVE}" "$OUT"
	done
fi