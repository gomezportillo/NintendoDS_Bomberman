/*-----------------------------------------------
Includes
-------------------------------------------------
*/

// Includes C
#include <stdio.h>

// Includes propietarios NDS
#include <nds.h>

// Includes librerias propias
#include <nf_lib.h>


/*
-------------------------------------------------
Métodos
-------------------------------------------------
*/

//los métodos antes del main, ya que como, a diferencia de Java, no se pueden declarar statics (que los cargue antes del main) hay que hacerlo manualmente (estúpido y sensual Java...)

//como no se le pueden pasar argumentos a los metodos de los timers, definimos las variables que almacenaran la posición del mapa en la que se producirá la explosión de manera global, y un par de variables más
s16 posExploX = 0;
s16 posExploY = 0;
bool anotherBomb = true;
s16 mapaY = 0;

void teselaBomba(s16 x, s16 y){ //cambia la tesela en la que está el sprite a la de la bomba
	NF_SetTileOfMap(1, 2, x/8, y/8, 2); //la dos será la tesela de la bomba porque el compilador así lo quiere, y nosotros, sus sumisos siervos, obedecemos
	NF_SetTile(0, x, y, 6); //la posición de la paleta de colores donde realmente está la bombah
	NF_UpdateVramMap(1,2);
}

//Metallica's And Justice for All patrocina este método
void explosionRojo(){ //a este método lo llama el TIMER0
	int i;
	s16 x=posExploX;
	s16 y=posExploY;
	bool stop=false; //variable para parar si encuentra un bloque negro, ya que la explosión se acercaria peligrosamente a los boundaries

	for(i=0;i<3 && !stop;i++){	//pos X izquierda chequea si no es gris oscuro o negro y lo cambia a rojo
		if (NF_GetTile(0,x+i, y)!=3 && NF_GetTile(0,x+i, y)!=5){
			NF_SetTile(0, x+i, y, 4); //cambia el mapa de colisiones
			NF_SetTileOfMap(1, 2, (x/8)+i, y/8, 1);
		}else stop=true;  //si encuentra un bloque negro, ya que la explosión se acerca peligrosamente a los boundaries,para todo con la variable booleana stop

	}
	stop=false; //reinicia la variable, ya que en las demás direcciones no tiene por qué haber un bloque negro tb

	for(i=0;i>-3 && !stop;i--){	//pos X izquierda chequea si no es gris oscuro o negro y lo cambia a rojo
		if (NF_GetTile(0,x+i, y)!=3 && NF_GetTile(0,x+i, y)!=5){
			NF_SetTile(0, x+i, y, 4); //cambia el mapa de colisiones
			NF_SetTileOfMap(1, 2, (x/8)+i, (y/8), 1);
		}else stop=true;
	}
	stop=false;

	for(i=0;i<3 && !stop;i++){	//pos Y arriba chequea si no es gris oscuro o negro y lo cambia a rojo
		if (NF_GetTile(0,x, y+i)!=3 && NF_GetTile(0,x, y+i)!=5){
			NF_SetTile(0, x, y+i, 4); //cambia el mapa de colisiones
			NF_SetTileOfMap(1, 2, (x/8), (y/8)+i, 1); //cambia el mapa visual
		}else stop=true;
	}

	stop=false;
	for(i=0;i>-3 && !stop;i--){	//pos Y abajo chequea si no es gris oscuro o negro y lo cambia a rojo
		if (NF_GetTile(0,x, y+i)!=3 && NF_GetTile(0,x, y+i)!=5){
			NF_SetTile(0, x, y+i, 4); //cambia el mapa de colisiones
			NF_SetTileOfMap(1, 2, (x/8), (y/8)+i, 1); //cambia el mapa de teselas (el visual)
		}else stop=true;
	}

	NF_UpdateVramMap(1,2); //actualiza la pantalla con el mapa indicado(1=pantalla de abajo; 2=capa a actualizar)
	TIMER_CR(0) = TIMER_DIV_1024 | TIMER_IRQ_REQ; //para el timer 0
	TIMER_DATA(1) = 0;
	TIMER_CR(1) = TIMER_DIV_1024 | TIMER_ENABLE| TIMER_IRQ_REQ; //activa el timer 1

}


void explosionVerde(){//a este método lo llama el TIMER1, y es el encargado de que vuelva a crecer la hierba donde antes hubo ceniza
	int i,j;
	s16 x= posExploX;
	s16 y= posExploY;

	//este método cambiará las teselas pertinentes a verde. Como la comprobación de que todo fuera correcto la hemos hecho en el método anterior, el de la explosión a rojo, aqui simplemente nos limitaremos a cambiar aquella tesela que estuviera en rojo a verde
	/*
	for(i=-3;i<=3 && x+i>0 && x+i<256;i++){ //pos X

	if (NF_GetTileOfMap(1, 2, (x/8)+i, y/8)==1){ //si es igual a rojo
	NF_SetTile(0, x+i, y, 4);  //mapa de colisiones
	NF_SetTileOfMap(1, 2, (x/8)+i, (y/8), 4); //mapa teselado (el que se veh)
}
}

for(i=-3;i<=3 && x+i>0 && x+i<256;i++){ //pos Y

if (NF_GetTileOfMap(1, 2, (x/8)+i, y/8)==1){ //si es igual a rojo
NF_SetTile(0, x, y+i, 4);
NF_SetTileOfMap(1, 2, (x/8), (y/8)+i, 4);
}
}
*/
for(i=0;i<256;i++){ //a po culo;recorre la imagen entera y cambia las rojas a verde
	for(j=0;j<256;j++){
		if(NF_GetTileOfMap(1, 2,(i/8),(j/8))==1){
			NF_SetTileOfMap(1, 2, (i/8), (j/8), 4); //mapa de teselitas
			NF_SetTile(0, i, j, 1);  //mapa de colisiones
		}
	}
}

NF_UpdateVramMap(1, 2);
TIMER_CR(1) = TIMER_DIV_1024 | TIMER_IRQ_REQ; //para el timer 1
anotherBomb=true; //puedes poner otra bomba

}


/*
-------------------------------------------------
Main() - Bloque general del programa:
-------------------------------------------------
*/


int main(int argc, char **argv) {

	//Declaración de los timers
	irqSet(IRQ_TIMER0, &explosionRojo); //temporizará el delay de la explosión
	irqEnable(IRQ_TIMER0);
	irqSet(IRQ_TIMER1, &explosionVerde); //temporizará el tiempo que están las teselas de la explosión, ya que después se pone verde
	irqEnable(IRQ_TIMER1);

	// Pantalla de espera inicializando NitroFS
	NF_Set2D(0, 0); // Modo 2D_0 en ambas pantallas
	NF_Set2D(1, 0);	 // Modo 2D_0 en ambas pantallas
	consoleDemoInit();
	iprintf("\n NitroFS init. Please wait.\n\n");
	iprintf(" Iniciando NitroFS,\n por favor, espere.\n\n");
	swiWaitForVBlank(); // Espera al sincronismo vertical

	// Define el ROOT e inicializa el sistema de archivos
	NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NITROFS

	// Inicializa el motor 2D (P: otra vez?)
	NF_Set2D(0, 0);				// Modo 2D_0 en ambas pantallas
	NF_Set2D(1, 0);

	// Inicializa el engine de audio de la DS
	soundEnable();

	// Inicializa los fondos tileados
	NF_InitTiledBgBuffers();	// Inicializa los buffers para almacenar fondos
	NF_InitTiledBgSys(0);		// Inicializa los fondos Tileados para la pantalla superior
	NF_InitTiledBgSys(1);		// Iniciliaza los fondos Tileados para la pantalla inferior

	// Inicializa los Sprites
	NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
	NF_InitSpriteSys(0);		// Inicializa los sprites para la pantalla superior
	NF_InitSpriteSys(1);		// Inicializa los sprites para la pantalla inferior

	// Inicializa el motor de texto
	NF_InitTextSys(0);			// Inicializa el texto para la pantalla superior

	// Inicializa los buffers de sonido
	NF_InitRawSoundBuffers();

	// Inicializa los buffers de mapas de colisiones
	NF_InitCmapBuffers();

	// Carga los archivos de fondo desde la FAT / NitroFS a la RAM
	NF_LoadTiledBg("bg/layer3", "bomberman", 256, 256);		// Carga el fondo para la capa 3, pantalla inferior
	NF_LoadTiledBg("bg/colmap", "maze", 256, 256);		// Carga el fondo para la capa 2, pantalla inferior
	NF_LoadTiledBg("bg/win", "win", 256, 256); //la imagen para cuando ganas
	NF_LoadTiledBg("bg/lose", "lose", 256, 256); //y para cuando pierdes, por supué

	/**/// Carga los archivos de sprites desde la FAT / NitroFS a la RAM
	NF_LoadSpriteGfx("sprite/puntero", 0, 8, 8);	// Puntero (file, id, width, height)
	NF_LoadSpritePal("sprite/puntero", 0); 		//file, ID del sprite al que pertenece dicha paleta
	NF_LoadSpriteGfx("sprite/enemy", 1, 8, 8);	//enemy
	NF_LoadSpritePal("sprite/enemy", 1);


	// Carga la fuente por defecto para el texto
	NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);

	// Carga los samples de sonido en formato RAW
	NF_LoadRawSound("sfx/sample", 0, 11025, 0);
	NF_LoadRawSound("sfx/music", 1, 22050, 0);

	// Carga el mapa de colisiones
	NF_LoadColisionMap("maps/cmap", 0, 256, 264); //ese 0 es el id del mapa, que será luego usado en el metodo getTite()

	// Crea los fondos de la pantalla superior
	NF_CreateTiledBg(0, 3, "bomberman"); //0 de la pantalla de arriba y 3 de la 3º capa
	// Crea los fondos de la pantalla inferior
	NF_CreateTiledBg(1, 2, "maze"); //laberinto

	// Crea una capa de texto
	NF_CreateTextLayer(0, 2, 0,	"normal");

	/**/// Transfiere a la VRAM los sprites necesarios
	NF_VramSpriteGfx(1, 0, 0, true);	//NF_VramSpriteGfx(u8 screen, u16 ram, u8 vram, bool keepframes);
	NF_VramSpritePal(1, 0, 0);		//screen,ramslot,vramslot
	NF_VramSpriteGfx(1, 1, 1, true);	//enemy
	NF_VramSpritePal(1, 1, 1);

	/**/// Crea el Sprite del muñequito y el enemigo en la pantalla inferior CreateSprite(pantalla, id_sprite, gfx, id_paleta, x, y);
	NF_CreateSprite(1, 0, 0, 0, 124, 92);	// Crea el puntero en la pantalla inferior
	NF_SpriteLayer(1, 0, 2);		// Y la capa sobre la que se dibujara (screen,id_sprite, layer)
	NF_CreateSprite(1, 1, 1, 1, 160,40); 	//enemy
	NF_SpriteLayer(1, 1, 2);


	// Variable para la lectura del keypad
	u16 keys = 0;

	// Variables para el control de movimiento
	s16 x = 128; //usada para calcular la posicion del sprite
	s16 y = 96;
	s16 spr_x = 0; //posicion del sprite
	s16 spr_y = 0;
	s16 enemy_x = 160; //posicion del enemigo
	s16 enemy_y = 40;
	s16 bg_x = 0; //posición del fondo
	s16 bg_y = 0;

	// Buffer de texto
	char mytext[32];

	// Variables de sonido
	u16 newpress = 0;
	u8 sound_id = 0;

	// Inicia la musica de fondo
	sound_id = NF_PlayRawSound(1, 127, 64, true, 0);

	//Cubre las teselas del fuego y la bomba,ya que estas deben estar en el mapa para luego poder usarlas
	NF_SetTileOfMap(1, 2, 0, 0, 3); //cubre el fuego con negro
	NF_SetTileOfMap(1, 2, 1, 0, 3); //cubre la bomba con negro
	NF_UpdateVramMap(1, 2);	//actualiza la pantalla

	// Boolean para continuar jugando, de momento useless
	bool youDie=false;
	bool enemyDies=false;

	//dos variables 2^2=4 para mover al enemy en las 4 direcciones posibles
	bool var1 = true;
	bool var2 = true;

	// Bucle (repite para siempre)

	while(!youDie && !enemyDies){

		//SONIDO
		// Detiene el sonido, pero no la borra de la memoria de sonido
		if (newpress & KEY_A) soundPause(sound_id);
		// Vuelve a reproducir el sonido, pero desde el principio
		if (newpress & KEY_B) soundResume(sound_id);
		// Para eliminar el sonido de la memoria de sonido (no del buffer de la libreria)
		// usa la funcion soundKill();

		// Reproduce el sonido
		if (newpress & KEY_R) NF_PlayRawSound(0, 127, 127, false, 0);
		if (newpress & KEY_L) NF_PlayRawSound(0, 127, 0, false, 0);

		// MOVIMIENTO DEL SPRITE
		scanKeys();
		unsigned down = keysDown();
		keys = keysHeld();
		if (keys & KEY_UP){
			y --;
			if (NF_GetTile(0,x, y)==3 || NF_GetTile(0,x, y)==2) y ++; //si la tesela es gris claro u oscuro, marcha atrás, primo
		}

		if (keys & KEY_DOWN){
			y ++;
			if (NF_GetTile(0,x, y)==3 || NF_GetTile(0,x, y)==2) y --;
		}

		if (keys & KEY_LEFT){
			x --;
			if(NF_GetTile(0,x, y)==3 || NF_GetTile(0,x, y)==2) x ++;
		}

		if (keys & KEY_RIGHT){
			x++;
			if (NF_GetTile(0,x, y)==3 || NF_GetTile(0,x, y)==2) x --;
		}

		//la cremita: las bombas
		// a partir de la posición del sprite en un radio de 3 a rojo y luego verde
		if (down & KEY_TOUCH && anotherBomb){

			posExploX=x; //posExploX/Y son variables globales, para que puedan acceder a ella los métodos de los TIMERS
			posExploY=y;
			anotherBomb=false; //no volverá a ser true hasta que acabe el proceso de la explosión de la bomba actual
			teselaBomba(posExploX, posExploY); //llama al método que pone la bomba, que a su vez llama a los demás en una vorágine de 			llamamiento a métodos hacinados
			TIMER_DATA(0) = 0;
			TIMER_CR(0) = TIMER_DIV_1024 | TIMER_ENABLE| TIMER_IRQ_REQ; //un segundo y llama al método que lo pondrá rojo

		}

		//aqui la "IA" que moverá al enemigo. Como si entrase en un bucle pararía el juego, habrá que hacerlo con ifs


		if(NF_GetTile(0,enemy_x, enemy_y)==1 && var1 && var2){ //si es verde, sigue
			enemy_x--;
			if(NF_GetTile(0,enemy_x, enemy_y)==4) enemyDies=true;	//si es fuego, muere
			if(NF_GetTile(0,enemy_x, enemy_y)!=1){  //si no es verde cambia de dirección
				var1=false; //con las cuatro posibles combinaciones de estas dos variables se computan todos los posibles movimientos
				var2=true;
				enemy_x++; //deshace el movimiento
				enemy_y--;} //prueba otra dirección

			}else if(NF_GetTile(0,enemy_x, enemy_y)==1 && !var1 && var2){
				enemy_y--;
				if(NF_GetTile(0,enemy_x, enemy_y)==4) enemyDies=true;
				if(NF_GetTile(0,enemy_x, enemy_y)!=1){
					var1=true;
					var2=false;
					enemy_x++;
					enemy_y++;}

				}else if(NF_GetTile(0,enemy_x, enemy_y)==1 && var1 && !var2){
					enemy_x++;
					if(NF_GetTile(0,enemy_x, enemy_y)==4) enemyDies=true;
					if(NF_GetTile(0,enemy_x, enemy_y)!=1){
						var1=false;
						var2=false;
						enemy_x--;
						enemy_y++;}

					}else if(NF_GetTile(0,enemy_x, enemy_y)==1 && !var1 && !var2){
						enemy_y++;
						if(NF_GetTile(0,enemy_x, enemy_y)==4) enemyDies=true;
						if(NF_GetTile(0,enemy_x, enemy_y)!=1){
							var1=true;
							var2=true;
							enemy_x--;
							enemy_y--;}
						}



						// limites del movimiento del muñequito
						if (x < 20) x = 20;
						if (x > 236) x = 236;
						if (y < 20) y = 20;
						if (y > 236 && (x<112 || x>128)) y = 236; //la puertecita de abajo



						//Para evitar que se repita el fondo 'til the infinite
						// POSICION DEL FONDO
						//bg_x = (x - 128); la x no se moverá, porque el ancho del mapa encaja en la pantalla
						//if (bg_x < 0) bg_x = 0;
						//if (bg_x > 254) bg_x = 254;
						mapaY=bg_y; //guardamos la posicion que tiene bg_y antes de cambiarla para poder hacer luego delta de bg_y
						bg_y = (y - 96);

						if (bg_y < 0) bg_y = 0;
						if (bg_y > 64) bg_y = 64; //lo justo para que no se vea el otro fondo repetido

						// Posicion de los sprites
						mapaY -= bg_y; //delta de bg_y QUE FUNCIONA!! QUÉ PUTO AMOO
						spr_x = (x - bg_x) - 4;
						spr_y = (y - bg_y) - 4;
						//enemy_x = (enemy_x + bg_x); //el mapa nunca se mueve en el eje X
						enemy_y= (enemy_y + mapaY);

						/**/

						NF_MoveSprite(1, 0, spr_x, spr_y); //posicion del muñequito (screen, spriteID, posX, posY)
						NF_MoveSprite(1, 1, enemy_x, enemy_y); //posicion del enemy IMPLEMENTAR SCRIPT QUE LO MUEVA

						int m;
						for(m=-4;m<=4;m++){ //para aumentar el radio de alcance del muñequito
							if(spr_x==enemy_x+m && spr_y==enemy_y+m) youDie=true;
						}

						// Imprime la posicion de ambos sprites
						sprintf(mytext,"x:%d  y:%d eX:%d eY:%d", x, y, enemy_x+4, enemy_y+4);
						NF_WriteText(0, 2, 1, 1, mytext);


						// Imprime el nº de tile en el que está el muñequito
						switch (NF_GetTile(0,x, y)) {
							case 0:
							sprintf(mytext,"Tile: Vacio");
							break;
							case 1:
							sprintf(mytext,"Tile: Verde");
							break;
							case 2:
							sprintf(mytext,"Tile: Gris Claro");
							break;
							case 3:
							sprintf(mytext,"Tile: Gris Oscuro");
							break;
							case 4:
							sprintf(mytext,"Tile: Rojo");
							youDie=true;
							break;
							case 5:
							sprintf(mytext,"Tile: Negro");
							break;
							case 6:
							sprintf(mytext,"Tile: Bomba");
							break;

						}//switch

						NF_WriteText(0, 2, 1, 3, mytext);

						NF_UpdateTextLayers();		// Actualiza las capas de texto

						NF_SpriteOamSet(1);		// Actualiza el Array del OAM

						swiWaitForVBlank();		// Espera al sincronismo vertical

						oamUpdate(&oamSub);		// Actualiza a VRAM el OAM Secundario

						NF_ScrollBg(1, 2, bg_x, bg_y);	// Actualiza el scroll

						NF_SpriteOamSet(0);		// Actualiza el Array del OAM
						NF_SpriteOamSet(1);		// Actualiza el Array del OAM

						// Dibuja los 3D Sprites
						NF_Draw3dSprites();
						// Actualiza la escena 3D, si no lo haces, no se mostrara en pantalla
						glFlush(0);

						oamUpdate(&oamSub);		// Actualiza a VRAM el OAM Secundario
						oamUpdate(&oamMain);		// Actualiza a VRAM el OAM Primario



					}//while
					//descargar el fondo del mapa y poner uno que diga YOI LOSE if youDie o YOU WIN if enemyDies
					NF_DeleteTiledBg(1, 2); //siempre
					//soundKill(sound_id); mejor que siga sonando la musica
					if(youDie){
						NF_CreateTiledBg(1, 2, "lose");
					}

					if(enemyDies){
						NF_CreateTiledBg(1, 2, "win");
					}

					while(true){ } // ...y más allá

					return 0;

				}//main
