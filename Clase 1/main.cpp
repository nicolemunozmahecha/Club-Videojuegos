#include <raylib.h>
#include <random>

int puntos_j1 = 0;
int puntos_j2 = 0;
bool juegoterminado = false;
Sound sonido_gol;

struct Bola{
        Vector2 pos = {};   // posicion
        Vector2 vel = {};   // velocidad
        int radio = 0;
        Color color = WHITE;
        
        void actualizar(){
            pos.x += vel.x;
            pos.y += vel.y;     // ctrl d
            
            if (pos.y + radio >= GetScreenHeight() or pos.y - radio < 0){
                    vel.y *= -1;
            }
            
            if (pos.x + radio >= GetScreenWidth()){
                    vel.x *= -1;
                    puntos_j1++;
                    PlaySound(sonido_gol);
                    reiniciar2();
                    
            }
            if (pos.x - radio < 0){
                    vel.x *= -1;
                    puntos_j2++;
                    PlaySound(sonido_gol);
                    reiniciar1();
            }
            

        };
        
        void reiniciar1(){       // para que despues de cada punto la bola nueva salga desde el centro
            pos = {640, 400};
            vel = {6, 6};
        }
        void reiniciar2(){       // para que despues de cada punto la bola nueva salga desde el centro
            pos = {640, 400};
            vel = {-6, 6};
        }
        
        void dibujar(){
            DrawCircle(pos.x, pos.y, radio, WHITE); 
        };
};

struct Pala{
    Rectangle rec = {};
    int vel = 0;                                // velocidad vertical
    
    void actualizar(){
        if (IsKeyDown(KEY_W)){                  // si dejas de pulsar la tecla w, para ir hacia arriba
            rec.y -= vel;   
        }
         if (IsKeyDown(KEY_S)){                  // si dejas de pulsar la tecla s, para ir hacia abajo
            rec.y += vel;   
        }
        comprobar_colision();
    }
    
    void actualizar2(){
        if (IsKeyDown(KEY_UP)){                  // si dejas de pulsar la tecla w, para ir hacia arriba
            rec.y -= vel;   
        }
         if (IsKeyDown(KEY_DOWN)){                  // si dejas de pulsar la tecla s, para ir hacia abajo
            rec.y += vel;   
        }
        comprobar_colision();  
    }
    
    void comprobar_colision(){
        if (rec.y <= 5) {
            rec.y = 5;
        }else if(rec.y >= GetScreenHeight() - rec.height -5){
            rec.y = GetScreenHeight() - rec.height -5;
        }
    }
    
    void dibujar(){
        DrawRectangle(rec.x, rec.y, rec.width, rec.height, WHITE);
        // tb podemos poner: DrawRectangle(rec, WHITE);
    }
};


int main(){
    // Paso 1. Dibujar la interfaz
    InitWindow(1280, 800, "PONG");
    InitAudioDevice();
    SetTargetFPS(60);                           // Para limitar el framebase del juego
    
    Sound sonido_golpes[2];
    sonido_golpes[0] = LoadSound("Sonidos/pongblipa4.wav");
    sonido_golpes[1] = LoadSound("Sonidos/pongblipa5.wav");
    sonido_gol = LoadSound("Sonidos/objective-complete.wav");
    
    Bola bola;
    bola.pos = Vector2{640, 400};
    bola.vel = Vector2{6, 6};
    bola.radio = 10;
    Pala pala;
    pala.rec = Rectangle{10, GetScreenWidth()/2-50, 25, 100};
    pala.vel = 10;
    Pala pala2;
    pala2.rec = Rectangle{GetScreenWidth() -  25 - 10, GetScreenWidth()/2-50, 25, 100};
    pala2.vel = 10;
    
    while(!WindowShouldClose()){                // Hasta que no presionemos el escape no sale del bucle
        // 2. EVENTOS
        //    ACTUALIZADO
        bola.actualizar();
        pala.actualizar();
        pala2.actualizar2();
        
        
        // Parámetros: mensaje, coordenadas, tamaño de la fuente, COLOR
        DrawText(TextFormat("J1: %i", puntos_j1), GetScreenWidth()*1/3 - 60, 10, 80, WHITE);
        DrawText(TextFormat("J2: %i", puntos_j2), GetScreenWidth()*2/3, 10, 80, WHITE);
        
        if (CheckCollisionCircleRec(bola.pos, bola.radio, pala.rec)){
            //bola.pos.x = pala.rec.x + pala.rec.width + bola.radio;
            bola.vel.x *= -1.1f;       // si quiero que vaya más rapido en cada colisión: -1.1f
            PlaySound(sonido_golpes[rand()%2]);
        }
        if (CheckCollisionCircleRec(bola.pos, bola.radio, pala2.rec)){
            //bola.pos.x = pala2.rec.x + pala2.rec.width + bola.radio;
            bola.vel.x *= -1.1f;
            PlaySound(sonido_golpes[rand()%2]);
        }
        
        if (puntos_j1 == 5 || puntos_j2 == 5) {
            juegoterminado = true;
        }

        if (juegoterminado){
            bola.pos.x = GetScreenWidth()/2;
            bola.pos.y = GetScreenHeight()/2;
            if (puntos_j1 == 5){
                DrawText(TextFormat("El jugador %d ha ganado el juego", 1), GetScreenWidth()/2-450, GetScreenHeight()/2, 60, RED);
            }
            else{
                DrawText(TextFormat("El jugador %d ha ganado el juego", 2), GetScreenWidth()/2-450, GetScreenHeight()/2, 60, RED);
            }
        }
        
        // 1. DIBUJO
        BeginDrawing();
        ClearBackground(BLACK);                 // Para que la bola no aparezca como en cursiva
        
        // Parámetros: inicio x, inicio y, final x, final y
        // Nos saldrá una linea de 1 pixel
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenWidth(), WHITE);
        
        // Parametros: Coord(x, y,), ancho, alto --> desde punto superior izquierdo
        DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 10, GRAY);
        // DrawRectangle(10, GetScreenHeight()/2-50, 25, 100, WHITE);
        
        // Dibujamos bola
        bola.dibujar();
        // Dibujamos pala
        pala.dibujar();
        pala2.dibujar();
        
        EndDrawing();
        
        
    }
    
    CloseWindow();


    return 0;
}