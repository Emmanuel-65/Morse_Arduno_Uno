#include <Arduino.h>
// MORSE_v3.ino  14-avr-2025

char message[]="HELLO ARDUINO";         // message > LED_BUILTIN, Serial                               
#define va_point 200                   // Valeur Absolue du point (dénominateur commun) en millisecondes
struct {char nb_even; char code; char longueur_en_points;} morse[] { // puisque cette structure est utilisée un seul fois, pas besion de lui donner de nom
  // nb_even : nombre d'évenement point ou tiret
  // code (exprimé en booléen) : 0b0 = point,  0b1 = tiret 				
  // longueur_en_points : temps nécessaire du caractère (calculé dans setup)
  {0},            //  0 Cet élément est les 31 suivants ne servent pas. Ils servent à mettre en phase les éléments et les codes ascii des caractères. 
                  //    Sinon on doit retrancher une certaine valeur à chaque utilisation de la structure !
  {0},            //  1
  {0},            //  2
  {0},            //  3
  {0},            //  4
  {0},            //  5
  {0},            //  6
  {0},            //  7
  {0},            //  8
  {0},            //  9
  {0},            // 10
  {0},            // 11
  {0},            // 12
  {0},            // 13
  {0},            // 14
  {0},            // 15
  {0},            // 16
  {0},            // 17
  {0},            // 18
  {0},            // 19
  {0},            // 20
  {0},            // 21
  {0},            // 22
  {0},            // 23
  {0},            // 24
  {0},            // 25
  {0},            // 26
  {0},            // 27
  {0},            // 28
  {0},            // 29
  {0},            // 30
  {0},            // 31
  {0},            // 32 [SPACE] pas utilisé (inter_mot)
  {0},            // 33 !
  {0},            // 34 "
  {0},            // 35 #
  {0},            // 36 $
  {0},            // 37 %
  {0},            // 38 &
  {0},            // 39 '
  {0},            // 40 (
  {0},            // 41 )
  {0},            // 42 *
  {0},            // 43 +
  {0},            // 44 ,
  {0},            // 45 -
  {0},            // 46 .
  {0},            // 47 /
  {5,0b11111},    // 48 0
  {5,0b01111},    // 49 1
  {5,0b00111},    // 50 2
  {5,0b00011},    // 51 3
  {5,0b00001},    // 52 4
  {5,0b00000},    // 53 5
  {5,0b10000},    // 54 6
  {5,0b11000},    // 55 7
  {5,0b11100},    // 56 8
  {5,0b11110},    // 57 9
  {0},            // 58 :
  {0},            // 59 ;
  {0},            // 60 <
  {0},            // 61 =
  {0},            // 62 >
  {8,0b00000000}, // 63 ?
  {0,0},          // 64 @
  {2,0b01},       // 65 A
  {4,0b1000},     // 66 B
  {4,0b1010},     // 67 C
  {3,0b100},      // 68 D
  {1,0b0},        // 69 E
  {4,0b0010},     // 70 F
  {3,0b110},      // 71 G
  {4,0b0000},     // 72 H
  {2,0b00},       // 73 I
  {4,0b0111},     // 74 J
  {3,0b101},      // 75 K
  {4,0b0100},     // 76 L
  {2,0b11},       // 77 M
  {2,0b10},       // 78 N
  {3,0b111},      // 79 O
  {4,0b0110},     // 80 P
  {4,0b1101},     // 81 Q
  {3,0b010},      // 82 R
  {3,0b000},      // 83 S
  {1,0b1},        // 84 T
  {3,0b001},      // 85 U
  {4,0b0001},     // 86 V
  {3,0b011},      // 87 W
  {4,0b1001},     // 88 X
  {4,0b1011},     // 89 Y
  {4,0b1100},     // 90 Z
  {0},            // 91 [
  {0},            // 92 [ANTISLASH]
  {0},            // 93 ]
  {0},            // 94 ^
  {0},            // 95 _
  {0},            // 96 `
  {0},            // 97 a les minuscules seront définis dans setup()
  {0},            // 98 b
  {0},            // 99 c
  {0},            //100 d
  {0},            //101 e
  {0},            //102 f
  {0},            //103 g
  {0},            //104 h
  {0},            //105 i
  {0},            //106 j
  {0},            //107 k
  {0},            //108 l
  {0},            //109 m
  {0},            //110 n
  {0},            //111 o
  {0},            //112 p
  {0},            //113 q
  {0},            //114 r
  {0},            //115 s
  {0},            //116 t
  {0},            //117 u
  {0},            //118 v
  {0},            //119 w
  {0},            //120 x
  {0},            //121 y
  {0},            //122 z
  {0},            //123 {
  {0},            //124 |
  {0},            //125 }
  {0},            //126 ~
  {0},            //127 [DEL] ou '' non utilisé
};
#define quid '?'                  // si morse inconnu
#define vr_tiret 3               // = 3 fois v_point                                               VALEUR RELATIVE DU TIRET
#define vr_inter_morse 1         //                                                                VALEUR RELATIVE RESPIRATION ENTRE IMPULSION
#define vr_inter_lettre 3        //                                                                VALEUR RELATIVE RESPIRATION ENTRE CHAQUE LETTRE 
#define vr_inter_mot 7-3         // -3 car v_inter_lettre déjà temporisé                           VALEUR RELATIVE RESPIRATION ENTRE CHAQUE MOT    v_inter_mot <=8
#define vr_inter_message  20       // grande respiration avant de recommencer

void setup() { 
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  morse[0].nb_even=vr_inter_mot; morse[0].code=0; morse[0].longueur_en_points=vr_inter_mot; // Cas spécial pour espace ou fin de mot
  for (int c=' '+1; c<127;c++) {
    if (morse[c].nb_even==0) morse[c]=morse[quid]; 
    morse[c].longueur_en_points=0;
    for (int m=morse[c].nb_even-1; m>=0; m--) { 
      morse[c].longueur_en_points += (morse[c].code & (1<<m)) ? vr_tiret : 1; 
      if (m) morse[c].longueur_en_points += vr_inter_morse;
    }
    morse[c].longueur_en_points +=vr_inter_lettre;
  } 
  for (int c='A'; c<='Z'; c++) morse[c-'A'+'a']=morse[c];
  while (!Serial) {; }
}
#define taille_buffer (3*8*vr_tiret)   // parenthèses importantes
class MyBuffer { public :
  bool M_tampon[taille_buffer];
  unsigned int entree; // fait que s'incrémenter à chaque nouveau caractère (si il y a la place)
  unsigned int sortie; // fait que s'incrémenter à chaque envoi de code (morse) (si il est temps)
  unsigned long prochain_envoi;
  bool etat_led;
  void RaZ() {digitalWrite(LED_BUILTIN, LOW); etat_led=false; entree=0; sortie=0; prochain_envoi=millis();
  }
  bool charge(char ascii) { bool reponse=true;   // return   ! false                   ! true
  //                                                ascii>0  ! M_tampon est plein      ! ascii à pu être chargé dans M_tampon
  //                                                ascii==0 ! M_tampon n'est pas vide ! M_tampon est vide (tout le message à été transmis)
    if (ascii) {
      if ((entree+morse[ascii].longueur_en_points-sortie) < taille_buffer) {
        if (ascii == ' ') for (int c=0; c<vr_inter_mot; c++) M_tampon[entree++ %(taille_buffer)]=false;
        else if (ascii > ' ' && ascii < 127) {
          for (int m=morse[ascii].nb_even-1; m>=0; m--) { 
            M_tampon[entree++ %taille_buffer]=true; 
            if (morse[ascii].code & (1<<m)) for (int c=1; c<vr_tiret; c++) M_tampon[entree++ %taille_buffer]=true; // c=1 car un true est deja mis ligne précédente
            if (m) for (int c=0; c<vr_inter_morse; c++) M_tampon[entree++ %taille_buffer]=false;        
          } 
          for (int c=0; c<vr_inter_lettre; c++) M_tampon[entree++ %taille_buffer]=false;           
        }
        else if (ascii) while(true) { digitalWrite(LED_BUILTIN, LOW); delay(va_point/10); digitalWrite(LED_BUILTIN, HIGH); delay(va_point/10); } //ERREUR ascii non autorisé
      }
      else reponse=false; // M_tampon est plein
    }
    else if (entree != sortie) reponse=false; // M_tampon n'est pas vide. Peut-être qu’il sera vide à la fin de la fonction suite au vidage du tampon.
                                              // Dans ce cas reponse passera à false au prochain passage de loop
    if (entree != sortie && millis() > prochain_envoi) { 
      if (etat_led != M_tampon[sortie %taille_buffer]) { 
        if (M_tampon[sortie %taille_buffer]) digitalWrite(LED_BUILTIN, HIGH); else digitalWrite(LED_BUILTIN, LOW);
        etat_led = M_tampon[sortie %taille_buffer];
      }
      if (M_tampon[sortie %taille_buffer]) Serial.print("_"); else Serial.print(" ");
      sortie++;
      prochain_envoi += va_point;  
    } 
    return reponse;
  }
};
MyBuffer buffer; 
int l=0;
void loop() {
  if (l==0) {
    Serial.println(" ");Serial.print(message);
    buffer.RaZ(); Serial.print(" ="); 
  }
  if (message[l]==0) { 
    if (buffer.charge(0)) { l=0; delay(vr_inter_message * va_point); } // delay pour simplifier      
  }
  else if (buffer.charge(message[l])) l++;                             
}


/*****************************************************************************************************************************************************************
**                                                                                                                             POUR Arduino Uno [ELEGOO UNO R3] **
** Le croquis utilise 3336 octets (10%) de l'espace de stockage de programmes. Le maximum est de 32256 octets.                                                   **
** Les variables globales utilisent 675 octets (32%) de mémoire dynamique, ce qui laisse 1373 octets pour les variables locales. Le maximum est de 2048 octets. **
*****************************************************************************************************************************************************************/
/*
En gros il y a : 

- une “struct” pour définir le code morse des caractères ascii 

- un setup qui met le code du point d’interrogation dans les caractères non renseigné puis calcule les longueur_en_points de chaque caractères. 

- une “class” qui va gérer une “mémoire tampon”, le clignotement de la led et la sortie en texte (via la fonction “charge”).

- une “loop” qui sans arrêt bombarde la fonction “charge” de “MyBuffer” : 
. de code ASCII de “HELLO ARDUINO” 
ou
. de zéro si tout le message est dans “buffet” mais que le code n’est pas entièrement envoyé 

Avant la fonction loop() il y a “int l=0;”. Celui-ci aurais pus être intégré dans la classe MyBuffer. 
Mais si on améliore le programme pour que le message soit saisie en direct au clavier ce “l” sera plus utile dans une fonction gérant l’entrée que la sortie…
Ou mieux s'en passer en détruisant le message en l'envoyant
*/
