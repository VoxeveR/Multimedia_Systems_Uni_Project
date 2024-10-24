SRC_DIR = sources

all:
	g++ -I src/include -L src/lib -o main \
		$(SRC_DIR)/SM2024-Projekt.cpp \
		$(SRC_DIR)/SM2024-Dithering.cpp \
		$(SRC_DIR)/SM2024-Funkcje.cpp \
		$(SRC_DIR)/SM2024-Konwersje.cpp \
		$(SRC_DIR)/SM2024-MedianCut.cpp \
		$(SRC_DIR)/SM2024-Modele.cpp \
		$(SRC_DIR)/SM2024-Paleta.cpp \
		$(SRC_DIR)/SM2024-Pliki.cpp \
		$(SRC_DIR)/SM2024-Zmienne.cpp \
		-lmingw32 -lSDL2main -lSDL2

run: all
	.\main.exe

clean:
	del main.exe