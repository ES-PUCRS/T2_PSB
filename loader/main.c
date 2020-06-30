#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <stdbool.h>    // Para usar boolean

// SOIL é a biblioteca para leitura das imagens
#include "include/SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (RGB*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: %s (%s)\n", SOIL_last_result(), name);
        exit(1);
    }
    printf("Load: %s (%d x %d x %d)\n", name, pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    Img base, secreta;
    if(argc < 3) {
        printf("loader [img base] [img secreta]\n");
        exit(1);
    }
    load(argv[1], &base);
    load(argv[2], &secreta);

    //earth.jpg british_summer.jpg
    for(int i = 0; i < base.width * base.height; i++){
        base.img[i].r &= 0b11111100;
        base.img[i].g &= 0b11111100;
        base.img[i].b &= 0b11111100;
    }

    bool trigger = false;
    int j = 0, l = 0;
    int diff = base.width - secreta.width;

    for(int i = 0, l = 0; i < base.width * base.height; i++, l++){
        if( l >= base.width ){
            l = 0;
            trigger = true;
        } else if (l > secreta.width){
            trigger = false;
        }

        if(trigger && j < secreta.width * secreta.height){
            base.img[i].r |= secreta.img[j].r >> 6;
            base.img[i].g |= secreta.img[j].g >> 6;
            base.img[i].b |= secreta.img[j].b >> 6;
            j++;
        }
    }

    printf("Teste: gravando imagem base em saida.bmp\n");
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP,
    base.width, base.height, 3, (const unsigned char*) base.img);

    free(base.img);
    free(secreta.img);
}
