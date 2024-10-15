.PHONY: clean iso all release

dats = build/ledgedash.dat build/wavedash.dat build/lcancel.dat build/labCSS.dat build/eventMenu.dat build/lab.dat build/slalom.dat

# find all .asm and .s files in the ASM dir. We have the escape the spaces, so we pipe to sed
ASM_FILES := $(shell find ASM -type f \( -name '*.asm' -o -name '*.s' \) | sed 's/ /\\ /g')

MEX_BUILD=mono MexTK/MexTK.exe -ff -b "build" -q -ow -l "MexTK/melee.link" -op 2
MEX_TRIM=mono MexTK/MexTK.exe -trim

clean:
	rm -rf TM-More.iso
	rm -rf ./build/

build/eventMenu.dat: src/events.c src/events.h
	cp "dats/eventMenu.dat" "build/eventMenu.dat" 
	$(MEX_BUILD) -i "src/events.c" -s "tmFunction" -dat "build/eventMenu.dat" -t "MexTK/tmFunction.txt"
	$(MEX_TRIM) "build/eventMenu.dat"

build/lab.dat: src/lab.c src/lab.h src/lab_common.h
	cp "dats/lab.dat" "build/lab.dat"
	$(MEX_BUILD) -i "src/lab.c" -s "evFunction" -dat "build/lab.dat" -t "MexTK/evFunction.txt"
	$(MEX_TRIM) "build/lab.dat"

build/labCSS.dat: src/lab_css.c src/lab_common.h
	cp "dats/labCSS.dat" "build/labCSS.dat"
	$(MEX_BUILD) -i "src/lab_css.c" -s "cssFunction" -dat "build/labCSS.dat" -t "MexTK/cssFunction.txt"
	$(MEX_TRIM) "build/labCSS.dat"

build/lcancel.dat: src/lcancel.c src/lcancel.h
	cp "dats/lcancel.dat" "build/lcancel.dat"
	$(MEX_BUILD) -i "src/lcancel.c" -s "evFunction" -dat "build/lcancel.dat" -t "MexTK/evFunction.txt"
	$(MEX_TRIM) "build/lcancel.dat"

build/ledgedash.dat: src/ledgedash.c src/ledgedash.h
	cp "dats/ledgedash.dat" "build/ledgedash.dat"
	$(MEX_BUILD) -i "src/ledgedash.c" -s "evFunction" -dat "build/ledgedash.dat" -t "MexTK/evFunction.txt"
	$(MEX_TRIM) "build/ledgedash.dat"

build/wavedash.dat: src/wavedash.c src/wavedash.h
	cp "dats/wavedash.dat" "build/wavedash.dat"
	$(MEX_BUILD) -i "src/wavedash.c" -s "evFunction" -dat "build/wavedash.dat" -t "MexTK/evFunction.txt"
	$(MEX_TRIM) "build/wavedash.dat"

build/slalom.dat: src/slalom.c src/slalom.h
	cp "dats/slalom.dat" "build/slalom.dat"
	$(MEX_BUILD) -i "src/slalom.c" -s "evFunction" -dat "build/slalom.dat" -t "MexTK/evFunction.txt"
	$(MEX_TRIM) "build/slalom.dat"

build/codes.gct: Additional\ ISO\ Files/opening.bnr $(ASM_FILES)
	cd "Build TM Codeset" && ./gecko build
	cp Additional\ ISO\ Files/* build/

build/Start.dol: | build
	./gc_fst read ${iso} Start.dol build/Start.dol
	xdelta3 -d -f -s build/Start.dol "Build TM Start.dol/patch.xdelta" build/Start.dol

TM-More.iso: build/Start.dol build/codes.gct $(dats)
	if [[ ! -f TM-More.iso ]]; then cp ${iso} TM-More.iso; fi
	./gc_fst fs TM-More.iso \
		delete MvHowto.mth \
		delete MvOmake15.mth \
		delete MvOpen.mth \
		insert TM/eventMenu.dat build/eventMenu.dat \
		insert TM/lab.dat build/lab.dat \
		insert TM/labCSS.dat build/labCSS.dat \
		insert TM/lcancel.dat build/lcancel.dat \
		insert TM/ledgedash.dat build/ledgedash.dat \
		insert TM/wavedash.dat build/wavedash.dat \
		insert TM/slalom.dat build/slalom.dat \
		insert codes.gct build/codes.gct \
		insert Start.dol build/Start.dol \
		insert opening.bnr build/opening.bnr
	./gc_fst set-header TM-More.iso "GTME01" "Melee Training Mode - More"

build:
	mkdir -p build

iso: TM-More.iso

TM-More.zip: TM-More.iso
	zip TM-More.zip TM-More.iso

release: TM-More.zip

all: iso release
