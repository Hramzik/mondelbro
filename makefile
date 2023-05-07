

cc        = gcc
cppfolder = cpp
exefolder = exe

define flags
	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
endef

define flags_sdl
	-IC:\Users\hramz_3vliuy6\Desktop\lib\SDL2\include -LC:\Users\hramz_3vliuy6\Desktop\lib\SDL2\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
endef

define flags_cut_bc_of_sdl
	-Wshadow -Winit-self -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
endef

define compile_sdl_opt
	$(sc) $(cc) $1 $2 $3 $4 $5 $6 -o $7 $(flags_sdl) $(flags_cut_bc_of_sdl) -O3 -mavx2
endef

define compile_sdl
	$(sc) $(cc) $1 $2 $3 $4 $5 -o $6 $(flags_sdl) $(flags_cut_bc_of_sdl)
endef

sc = @ #showcommands
default_name = prog
default_path = $(exefolder)/$(default_name)


all: opt


mdl:
	$(call compile_sdl,     $(cppfolder)/main.cpp, $(cppfolder)/graphics.cpp, \
	$(cppfolder)/mdl.cpp, $(cppfolder)/mdl2.cpp, $(cppfolder)/mdl3.cpp, $(default_path))

opt:
	$(call compile_sdl_opt, $(cppfolder)/main.cpp, $(cppfolder)/graphics.cpp, \
	$(cppfolder)/mdl.cpp, $(cppfolder)/mdl2.cpp, $(cppfolder)/mdl3.cpp, $(cppfolder)/mdl4.cpp, $(default_path))

