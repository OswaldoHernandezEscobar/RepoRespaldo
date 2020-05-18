/*------------------ Proyecto Computación Gráfica e Interacción Humano Computadora -----------------------------*/
/* ---------------- Lego Metal Fest ----------------------*/

/* ---------------- Integrantes: ----------------------*/
/* ---------------- Gallegos Santiago Jair de Jesús ----------------------*/
/* ---------------- Hernández Escobar Oswaldo ----------------------*/

//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h> 
#include <stb_image.h>
#include <windows.h>
#include <MMSystem.h>
#include "camera.h"
#include "Model.h"


// Other Libs
#include "SOIL2/SOIL2.h"


void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint skyboxVBO, skyboxVAO;

float escala = 0.3f; //controla el tamño de la banda
bool noche = true;

//Camera
Camera camera(glm::vec3(0.0f, 4.0f, 13.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(100.0f, 1.0f, -100.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);


void myData(void);
void display(Shader, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

bool recorrido = false;
//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f,
		ovX1 = -10.0f,
		movY1 = 0.0f,
		movZ1 = -30.0f,
		rotY = 0.0f,
		rotX1 = 0.0f,
		rotZ = 0.0f,
		movCabeza = 0.0f,
		rasgeo = -45.0f,
		movBrazoVoc = 0.0f,
		movBrazoBat = 0.0f;
bool toca = false, adelante = true, enfrente = true, mueveMono = false, mueveMono2 = false;
int contador = -1, musica = 0;

//Texture
unsigned int
		t_smile,
		t_pies,
		t_nara,
		t_pizza,
		t_toalla,
		t_unam,
		t_white,	
		t_aspros,
		t_madera,
		t_lego,
		t_mango,
		t_pede,
		t_micro,
		t_gui,
		t_cara3,
		t_lego_amarilla,
		t_poste,
		t_grande,
		t_agua,
		t_cielo,
		t_hastas,
		t_metal,
		t_kaw,
		t_bandera,
		t_brat,
		t_techo,
		t_queen,
		t_radio,
		t_tan,
		t_puerta,
		t_pantalon,
		t_metalli,
		t_rolli,
		t_bano,
		t_tacos,
		t_cue,
		t_cerv,
		t_bom,
		t_sushi,
		t_beat,
		t_ac,
		t_car,
		t_iron,
		t_cara4,
		t_azul,
		t_gus,
		t_play,
		t_pink,
		t_cara2,
		t_bat,
		t_carp,
		t_cancer;
	
//Keyframes Variables de dibujo para la animacion
float	posX = 0.0f, 
		posY = 0.0f, 
		posZ = 0.0f, 
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		movhead = 0.0f,
		movBrazo = 0.0f;

//Incrementos
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoInc = 0.0f,
		movimientoMono = -30.0f,
		movimientoMono2 = -30.0f,
		movimientoMono3 = -30.0f,
		desplazaMono = 0.0f,
		caeMonito = 0.0f,
		caeMonito2 = 0.0f,
		monitoX = 0.0f,
		monitoY = 0.0f;

float movLuzX = 0.0f;

float	luzRoja = 0.5f,
		luzVerde = 0.2f,
		luzAzul = 0.7f,
		r = 0.0f,
		g = 0.0f,
		b = 0.0f;

short estado = 0;

#define MAX_FRAMES 10
//cuadros intemedios
int i_max_steps = 15;

int i_curr_steps = 0;


typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazo;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false, baja = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].movBrazo = movBrazo;


	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazo = KeyFrame[0].movBrazo;

}

void interpolation(void)
{

	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	movBrazoInc = (KeyFrame[playIndex + 1].movBrazo - KeyFrame[playIndex].movBrazo) / i_max_steps;

}


void sonido() {
		
		sndPlaySound("musica.wav", SND_ASYNC);
	}



void sonido();



unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << filename << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{
	glActiveTexture(GL_TEXTURE2);
	t_brat = generateTextures("Texturas6/brazos_yDedos.png", 1);
	t_bom = generateTextures("Texturas6/bomboBateria.png", 1);
	t_tan = generateTextures("Texturas6/tamborcitos.jpg", 0);
	t_smile = generateTextures("Texturas6/awesomeface.png", 1);
	t_toalla = generateTextures("Texturas6/toalla.tga", 0);
	t_unam = generateTextures("Texturas6/escudo_unam.png", 1);
	t_white = generateTextures("Texturas6/white.jpg", 0);
	t_lego = generateTextures("Texturas6/Texturanegra_lego.png", 0);
	t_aspros = generateTextures("Texturas6/Aspros.jpg", 0);
	t_cancer = generateTextures("Texturas6/cancer.tga", 1);
	t_madera = generateTextures("Texturas6/Madera.png", 0);
	t_lego_amarilla = generateTextures("Texturas6/Lego_amarrillo.jpg", 0);
	t_cielo = generateTextures("Texturas6/Panorama.jpg", 0);
	t_cara3 = generateTextures("Texturas6/Cara3.png", 1);
	t_cara4 = generateTextures("Texturas6/Cara4.png", 1);
	t_grande = generateTextures("Texturas6/Legos.jpg", 0);
	t_hastas = generateTextures("Texturas6/Hastas.png", 0);
	t_metal = generateTextures("Texturas6/Metal.jpg", 0);
	t_poste = generateTextures("Texturas6/Poste_metalico.jpg", 0);
	t_techo = generateTextures("Texturas6/Techo.jpg", 0);
	t_bandera = generateTextures("Texturas6/bandera.jpg", 0);
	t_metalli = generateTextures("Texturas6/Metallica.jpg", 0);
	t_nara = generateTextures("Texturas6/Naranja.jpg", 0);
	t_micro = generateTextures("Texturas6/micro.jpg", 0);
	t_queen = generateTextures("Texturas6/Queen.jpg", 0);
	t_tacos = generateTextures("Texturas6/Tacos.jpg", 0);
	t_pizza = generateTextures("Texturas6/Pizza.jpg", 0);
	t_rolli = generateTextures("Texturas6/Rolling.jpg", 0);
	t_sushi = generateTextures("Texturas6/Sushi.jpg", 0);
	t_kaw = generateTextures("Texturas6/Kawai.jpg", 0);
	t_cerv = generateTextures("Texturas6/Cerveza.png", 0);
	t_gus = generateTextures("Texturas6/GUSN.jpg", 0);
	t_iron = generateTextures("Texturas6/IRON.jpg", 0);
	t_beat = generateTextures("Texturas6/Beatle.jpg", 0);
	t_ac = generateTextures("Texturas6/ac.jpg", 0);
	t_pink = generateTextures("Texturas6/Pink.jpg", 0);
	t_radio = generateTextures("Texturas6/Nirvana.jpg", 0);
	t_azul = generateTextures("Texturas6/Lego_azul.png", 0);
	t_agua = generateTextures("Texturas6/Agua.jpg", 0);
	t_agua = generateTextures("Texturas6/Agua.jpg", 0);
	t_puerta = generateTextures("Texturas6/Puerta.png", 1);
	t_bano = generateTextures("Texturas6/Bano.jpg", 0);
	t_car = generateTextures("Texturas6/Cara1.png", 1);
	t_cue = generateTextures("Texturas6/cuello-Piel.png", 1);
	t_play = generateTextures("Texturas6/playeras.jpg", 0);
	t_pantalon = generateTextures("Texturas6/pantalones.png", 1);
	t_pies = generateTextures("Texturas6/pies.jpg", 0);
	t_mango = generateTextures("Texturas6/mangoGuitarra.png", 1);
	t_gui = generateTextures("Texturas6/guitarra.png", 1);
	t_cara2 = generateTextures("Texturas6/Cara2.png", 1);
	t_pede = generateTextures("Texturas6/pedestalMicro.png", 1);
	t_bat = generateTextures("Texturas6/pedestalMicro.png", 1);
	t_carp = generateTextures("Texturas6/Carpa.png", 1);
}

void myData()
{
	
float vertices[] = {
	-0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 0.5f,0.5f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 0.5,0.5f,
	0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0f,2.0f,
	-0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f,2.0f,
		-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.5f,0.5f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.5f,0.5f,
	0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 2.0f,0.0f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	-0.5f,0.0f,-0.5f,1.0f,1.0f,1.0f, 1.0f,1.0f,
	0.5f,0.0f,-0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,
	0.0f,0.866f,-0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,
	-0.5f, 0.0f,0.5f,1.0f,1.0f,1.0f, 1.0f,1.0f,
	0.0f,0.866f,0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,
	0.0f,0.866f,-0.5f,1.0f,1.0f,1.0f,0.9f,1.0f,
	0.5f,0.0f,0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,
	-0.5f,0.0f,0.5f,1.0f,1.0f,1.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f, 1.0f,1.0f,1.0f,1.0f,0.0f,
	1.0f,0.0f,0.0f,1.0f,1.0f,1.0f, 0.75f,0.0f,
	0.7071f,0.0f,0.7071f,1.0f,1.0f,1.0f, 0.875f,0.0f,
	0.0f,0.0f,1.0f,1.0f,1.0f,1.0f, 1.0f,0.0f,
	-0.7071f,0.0f,0.7071f, 1.0f,1.0f,1.0f,0.125f,0.0f,
	-1.0f,0.0f,0.0f, 1.0f,1.0f,1.0f,0.25f,0.0f,
	-0.7071f,0.0f,-0.7071f,1.0f,1.0f,1.0f, 0.375f,0.0f,
	0.0f,0.0f,-1.0f,1.0f,1.0f,1.0f, 0.5f,0.0f,
	0.7071f,0.0f,-0.7071f, 1.0f,1.0f,1.0f,0.625f,0.0f,
	0.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0,1.0f,
	1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.75f,1.0f,
	0.7071f,1.0f,0.7071f,1.0f,1.0f,1.0f,0.875f,1.0f,
	0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
	-0.7071f,1.0f,0.7071f,1.0f,1.0f,1.0f,0.125f,1.0f,
	-1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.25f,1.0f,
	-0.7071f,1.0f,-0.7071f,1.0f,1.0f,1.0f,0.375f,1.0f,
	0.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,
	0.7071f,1.0f,-0.7071f,1.0f,1.0f,1.0f,0.625f,1.0f,
	-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	0.5f,-0.5f,0.5f, 1.0f,1.0f,1.0f,2.0f,0.0f,
	0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 2.0f,1.0f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,1.0f,
	-0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	-0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f,1.0f,
	0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0f,1.0f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0f,0.0f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.1736f,0.5390f,
	0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.1736f,0.5390f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.1736f,0.5390f,
	-0.5f,-0.5f,0.5f, 1.0f,1.0f,1.0f,0.1699f,0.1816f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.1699f,0.1816f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.1699f,0.1816f,
	-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,1.0f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,1.0f,
	0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	-0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,1.0f,
	-0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,0.0f,
	0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,0.0f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,1.0f,
	-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.4238f,0.5976,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.5644f,0.5976,
	0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.5644f,0.7148f,
	-0.5f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.4238f,0.7148f,
	-0.5f,-0.5f,-0.5f, 1.0f,1.0f,1.0f, 0.0f,0.0f,
	-0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f,1.0,
	0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f,2.0f,1.0f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f,  2.0f,0.0f,
	-0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0,0.0f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0f,0.0f,
	0.25f,0.5f,-0.5f, 1.0f,1.0f,1.0f,1.5f,4.0f,
	-0.25f,0.5f,-0.5f,1.0f,1.0f,1.0f, 0.5f,4.0f,
		-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f,2.0f,0.0f,
	0.25f,0.5f,0.5f,1.0f,1.0f,1.0f, 1.5f,1.0f,
	-0.25f,0.5f,0.5f,1.0f,1.0f,1.0f, 0.5f,1.0f,
	-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.0f,0.0f,
	0.0f,0.866f,0.5f,1.0f,1.0f,1.0f, 0.0,1.0,
	-0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,0.0,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 1.0f,0.0,
	0.0f,0.866f,-0.5f, 1.0f,1.0f,1.0f,1.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 1.0f,1.0f,1.0f,0.0f,0.0f,
	0.5f,-0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0,0.0f,
	0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 2.0f,4.0f,
	-0.5f,0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f,4.0f,
		-0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.36f,0.0f,
	0.5f,-0.5f,0.5f,1.0f,1.0f,1.0f, 0.4,0.0f,
	0.5f,0.5f,0.5f, 1.0f,1.0f,1.0f,0.4f,4.0f,
	-0.5f,0.5f,0.5f, 1.0f,1.0f,1.0f,0.36f,4.0f,
			-0.5f, -0.5f,-0.5f,1.0f,1.0f,1.0f, 0.36f, 0.0f,
			0.5f, -0.5f, -0.5f,1.0f,1.0f,1.0f, 0.4, 0.0f,
			0.5f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 0.4f, 4.0f,
			-0.5f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 0.36f, 4.0f,
		-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f,1.0f,1.0f,0.0f, 4.0f,
		-0.5f, -0.5f, -0.5f,1.0f,1.0f,1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 1.0f, 4.0f,
		0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 4.0, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 4.0f, 4.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 4.0, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 4.0f, 4.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.25, 0.66,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.66,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.34f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.25f, 0.34f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.75f, 0.34f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.75f, 0.66f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.66f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.34f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.66f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.34f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.25f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 10.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 10.0f, 10.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 10.0f, 0.0,
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0, 1.0f,
		-0.7071f, 1.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.125f, 1.0f,
		-0.7071f, 0.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.125f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5, 0.5f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 4.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 4.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
				-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f,
				-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.945f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.945f, 0.0f,

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.7242f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.7242f,

			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V0 - Frontal
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V1
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V5
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V4

			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V2 - Trasera
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V3
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V7
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V6
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V4 - Izq
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V7
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V3
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V0

			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V5 - Der
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V1
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V2
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V6

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V4 - Sup
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V5
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V6
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7

			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V0 - Inf
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V3
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V2
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V1

			-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,	//V0 - Frontal
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,	//V1
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	//V5
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	//V4

			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V2 - Trasera
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V3
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V6

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V4 - Izq
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V3
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V0

			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V5 - Der
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V1
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V2
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V6

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V4 - Sup
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V5
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V6
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7

			-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	//V0 - Inf
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	//V3
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	//V2
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	//V1

			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V0 - Frontal
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V1
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V5
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V4

			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	//V2 - Trasera
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	//V3
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//V6

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V4 - Izq
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V3
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V0

			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V5 - Der
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V1
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V2
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V6

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 00.0f, 0.0f,	//V4 - Sup
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V5
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V6
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V7

			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V0 - Inf
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V3
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V2
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//V1

			0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
			1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.94f, 0.5f,
			0.7071f, 0.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.8f, 0.833f,
			0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.9562f,
			-0.7071f, 0.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.1563f, 0.833f,
			-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0375f, 0.5f,
			-0.7071f, 0.0f, -0.7071f, 1.0f, 1.0f, 1.0f, 0.09375f, 0.3166f,
			0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0666f,
			0.7071f, 0.0f, -0.7071f, 1.0f, 1.0f, 1.0f, 0.8f, 0.3166f,
				0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.94f, 0.5f,
				0.7071f, 1.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.8f, 0.833f,
				0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.9562f,
				-0.7071f, 1.0f, 0.7071f, 1.0f, 1.0f, 1.0f, 0.1563f, 0.833f,
				-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0375f, 0.5f,
				-0.7071f, 1.0f, -0.7071f, 1.0f, 1.0f, 1.0f, 0.09375f, 0.3166f,
				0.0f, 1.0f, -1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0666f,
				0.7071f, 1.0f, -0.7071f, 1.0f, 1.0f, 1.0f, 0.8f, 0.3166f,


};
unsigned int indices[] = {

	0,1,2,
	0,3,2,
	1,2,5,
	5,2,6,
	7,4,5,
	7,5,6,
	7,3,4,
	0,3,4,
	0,1,4,
	1,4,5,
	3,2,7,
	7,2,6,

	8,9,10,

	8,11,12,
	12,13,8,
	12,13,9,
	9,14,12,
	14,12,15,
	9,8,11,
	9,14,15,

	16,17,18,
	16,18,19,
	16,19,20,
	16,20,21,
	16,21,22,
	16,22,23,
	16,23,24,
	16,24,17,

	25,26,27,
	25,27,28,
	25,28,29,
	25,29,30,
	25,30,31,
	25,31,32,
	25,32,33,
	25,33,26,

	19,28,27,
	19,18,27,
	18,27,26,
	18,17,26,
	17,26,33,
	17,24,33,
	24,32,33,
	23,24,32,
	23,31,32,
	23,22,31,
	22,31,30,
	30,21,22,
	30,29,21,
	20,21,29,
	29,28,20,
	19,20,28,

	34,35,36,
	37,36,34,
	37,34,39,
	39,38,34,
	35,36,40,
	35,40,41,
	38,39,40,
	40,41,38,
	34,35,38,
	38,41,35,
	39,37,36,
	36,40,39,
	42,43,44,
45,46,47,

		48,49,50,
		50,51,48,
		52,53,54,
		54,55,52,
		49,54,50,
		49,55,54,
		51,52,53,
		48,52,51,
		56,57,58,
		56,58,59,

			64,65,66,
		64,66,67,
		68,69,70,
		70,71,68,
		65,66,69,
		66,69,70,
		64,67,68,
		67,68,71,
		64,65,68,
		65,68,69,
		66,70,71,
		66,67,71,

		72,73,74,
		75,76,77,
		73,74,77,
		73,76,77,
		72,74,75,
		74,75,77,
		72,73,75,
		73,76,75,
		34,35,36,
		34,37,36,
		35,36,41,
		36,41,40,
		34,37,38,
		38,37,39,
		38,39,41,
		39,40,41,
		48,51,52,
		52,51,53,
		49,50,54,
		54,55,49,
		78,79,80,
		78,80,81,
		82,83,84,
		82,84,85,
		86,87,88,
		86,88,89,
		90,91,92,
		91,92,93,
		94,95,96,
		95,96,97,
		98,99,100,
		98,100,101,
		102,103,104,
		103,104,105,
		106,107,108,
		107,108,109,
		110,111,112,
		110,112,113,
		114,115,116,
		114,117,116,
		115,116,118,
		118,119,115,
		118,119,120,
		118,120,121,
		114,117,123,
		114,122,123,
		117,116,124,
		116,125,124,
		114,115,126,
		126,127,115,
		128,129,130,
		130,131,129,

		132, 133, 134,
		134, 135, 132,
		138,139,142,
		142,143,139,
		144,145,146,
		144,146,147,
		148,149,150,
		148,150,151,
		152,153,154,
		154,153,155,
		156,157,158,
		158,159,157,
		160,161,162,
		160,162,163,
		164,165,166,
		164,166,167,
		174,175,176,
		174,176,177,
		178,179,180,
		178,180,181,
		182,183,184,
		184,183,185,
		186,187,188,
		187,188,189,
		263,264,265,
		263,265,266,
		263,266,267,
		263,267,268,
		263,268,269,
		263,269,270,
		263,270,271,
		263,271,264,
		272,273,274,
		272,274,275,
		272,275,276,
		272,276,277,
		272,277,278,
		272,278,279,
		272,279,280,
		272,280,273,


};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;
			movBrazo += movBrazoInc;

			i_curr_steps++;

		}
	}

	if (toca) {
		if (adelante) {
			if (rasgeo <= 0.0f) {
				rasgeo += 1.0f;

			}
			else
			{
				adelante = false;
			}

		}

		else {
			if (rasgeo >= -45.0f) {
				rasgeo -= 1.0f;


			}

			else
			{
				adelante = true;
			}
		}


		if (enfrente) {
			if (movCabeza <= 0.0f) {
				movCabeza += 1.0f;

			}
			else
			{
				enfrente = false;
			}

		}

		else {
			if (movCabeza >= -45.0f) {
				movCabeza -= 1.0f;


			}

			else
			{
				enfrente = true;
			}
		}
	}

	if (recorrido) {

		if (contador <= 140) {
			contador++;
			deltaTime -= 0.4f;
			camera.ProcessKeyboard(FORWARD, (float)deltaTime / 100);

		}

		else if (contador <= 220) {

			deltaTime += 0.4f;
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime * (-2.0f) / 100);
			camera.ProcessKeyboard(LEFT, (float)deltaTime / 100);
			contador++;
		}


		else if (contador <= 250) {

			deltaTime += 0.4f;
			camera.ProcessKeyboard(FORWARD, (float)deltaTime / 100);
			contador++;
		}


		else if (contador <= 300) {

			deltaTime += 0.4f;
			camera.ProcessKeyboard(RIGHT, (float)deltaTime / 100);
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime * 0.25 / 100);
			contador++;
		}

		else if (contador <= 350) {

			deltaTime += 0.4f;
			camera.ProcessKeyboard(RIGHT, (float)deltaTime / 100);
			//camera.ProcessKeyboard(BACKWARD, (float)deltaTime*0.25);
			contador++;
		}

		else if (contador <= 420) {

			deltaTime += 0.4f;
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime / 100);
			contador++;
		}

		else {
			contador = 0;
			recorrido = false;
		}



	}
	if (toca) {
		if (adelante) {
			if (rasgeo <= 0.0f) {
				rasgeo += 1.0f;

			}
			else
			{
				adelante = false;
			}

		}

		else {
			if (rasgeo >= -45.0f) {
				rasgeo -= 1.0f;


			}

			else
			{
				adelante = true;
			}
		}


		if (enfrente) {
			if (movCabeza <= 0.0f) {
				movCabeza += 1.0f;

			}
			else
			{
				enfrente = false;
			}

		}

		else {
			if (movCabeza >= -45.0f) {
				movCabeza -= 1.0f;


			}

			else
			{
				enfrente = true;
			}
		}
	}

	if (mueveMono) {

		if (movimientoMono <= 30.0f) {
			movimientoMono += 1.0;
			movimientoMono2 -= 1.2;

		}

		else {

			if (desplazaMono <= 1.5f) desplazaMono += 0.2f;

			else if (caeMonito >= -90.0f) {
				caeMonito -= 1.0f;

			}

			else if (caeMonito2 >= -2.0) caeMonito2 -= 0.1f;

			else {
				mueveMono = false;
			}
		}
	}


	if (mueveMono2) {

		if (baja == false) {

			if (monitoY <= 1.0f) {

				monitoY += 0.2;
				monitoX += 0.5;

			}

			else baja = true;
		}

		else {

			if (monitoY >= 0) monitoY -= 0.2;

			else if (monitoX >= 0) monitoX -= 0.5;

			else {

				baja = false;
			}

		}
	}
	
}

void display(Shader shader,Model lego1, Model lego2, Model lego3, Model lego4, Model lego5, Model lego6, Model lego7, 
			 Model camarografo, Model carro)
{
	if (noche == true) {
		shader.use();

		shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("dirLight.direction", lightDirection);
		shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//shader.setVec3("pointLight.position", lightPosition);
		shader.setVec3("pointLight[0].position", glm::vec3(sin(r), sin(g), sin(b)));
		shader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setFloat("pointLight[0].constant", 1.0f);
		shader.setFloat("pointLight[0].linear", 0.009f);
		shader.setFloat("pointLight[0].quadratic", 0.032f);

		shader.setVec3("pointLight[1].position", glm::vec3(sin(r * 2), sin(g * 5), sin(b * 7)));
		shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[1].constant", 1.0f);
		shader.setFloat("pointLight[1].linear", 0.009f);
		shader.setFloat("pointLight[1].quadratic", 0.032f);

		shader.setVec3("pointLight[2].position", glm::vec3(sin(r * 9), sin(g * 7), sin(b * 4)));
		shader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[2].constant", 1.0f);
		shader.setFloat("pointLight[2].linear", 0.009f);
		shader.setFloat("pointLight[2].quadratic", 0.032f);

		shader.setVec3("pointLight[3].position", glm::vec3(luzRoja, luzVerde + 10.0f, luzAzul - 39.0f));
		shader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[3].diffuse", glm::vec3(luzRoja, luzVerde, luzAzul));
		shader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[3].constant", 1.0f);
		shader.setFloat("pointLight[3].linear", 0.009f);
		shader.setFloat("pointLight[3].quadratic", 0.032f);
	}
	else {
		shader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));

		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("dirLight.direction", lightDirection);
		shader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//shader.setVec3("pointLight.position", lightPosition);
		shader.setVec3("pointLight[0].position", glm::vec3(sin(r), sin(g), sin(b)));
		shader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setFloat("pointLight[0].constant", 1.0f);
		shader.setFloat("pointLight[0].linear", 0.009f);
		shader.setFloat("pointLight[0].quadratic", 0.032f);

		shader.setVec3("pointLight[1].position", glm::vec3(sin(r * 2), sin(g * 5), sin(b * 7)));
		shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[1].constant", 1.0f);
		shader.setFloat("pointLight[1].linear", 0.009f);
		shader.setFloat("pointLight[1].quadratic", 0.032f);

		shader.setVec3("pointLight[2].position", glm::vec3(sin(r * 9), sin(g * 7), sin(b * 4)));
		shader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[2].constant", 1.0f);
		shader.setFloat("pointLight[2].linear", 0.009f);
		shader.setFloat("pointLight[2].quadratic", 0.032f);

		shader.setVec3("pointLight[3].position", glm::vec3(luzRoja, luzVerde + 10.0f, luzAzul - 39.0f));
		shader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[3].diffuse", glm::vec3(luzRoja, luzVerde, luzAzul));
		shader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight[3].constant", 1.0f);
		shader.setFloat("pointLight[3].linear", 0.009f);
		shader.setFloat("pointLight[3].quadratic", 0.032f);
	}
	
	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 modelaux = glm::mat4(1.0f);
	glm::mat4 modelaux2 = glm::mat4(1.0f);
	glm::mat4 modelaux3 = glm::mat4(1.0f);
	glm::mat4 modelaux4 = glm::mat4(1.0f);
	glm::mat4 manoIzq = glm::mat4(1.0f);
	glm::mat4 guitarra = glm::mat4(1.0f);
	glm::mat4 origen = glm::mat4(1.0f);
	glm::mat4 cinturon = glm::mat4(1.0f);
	glm::mat4 bateria = glm::mat4(1.0f);
	glm::mat4 modelTemp = glm::mat4(1.0f);
	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	glBindTexture(GL_TEXTURE_2D, t_lego);
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.5f, 8.5f, 0.0f));
	//modelaux3 = modelaux = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Escenario principal
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	modelaux = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(16.0f, 2.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_play);

	//------------------------------ inicio banda  ------------------------------------------------
	model = glm::translate(modelaux, glm::vec3(-5.5f, 3.5625f, 0.0f));
	origen = model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	//model = glm::scale(model, glm::vec3(1.0f*escala, 1.0f*escala, 1.0f*escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);

	modelTemp = model = glm::translate(modelaux, glm::vec3(0.0f * escala, 2.25f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_car);
	model = glm::translate(modelTemp, glm::vec3(0.0f * escala, 1.0f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	guitarra = cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(0.0f * escala, -2.5f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	guitarra = model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der
	glBindTexture(GL_TEXTURE_2D, t_pies);
	//------------------------------------------------------------------------
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(-3.5f * escala, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));

	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	modelTemp = model = glm::translate(modelaux, glm::vec3(3.5f * escala, 1.5f * escala, 0.5f));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	//modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	//guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f, -2.25f, 0.25f));
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, 0.25f * escala));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3
	//-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_gui);
	modelTemp = model = glm::translate(guitarra, glm::vec3(2.5f * escala, 2.7f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, glm::vec3(2.5f * escala, 2.5f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1
	//---------------------------
	glBindTexture(GL_TEXTURE_2D, t_mango);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.2f * escala, 4.75f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f * escala, 7.0f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1

	//-----------------------------------------------------------------------------------------
	//							FIN DEL GUITARRISTA
	//-----------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(15.0f * escala, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(15.0f * escala, 2.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara2);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(15.0f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der
	glBindTexture(GL_TEXTURE_2D, t_pies);
	//------------------------------------------------------------------------
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(11.5f * escala, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(18.5f * escala, 1.5f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der1


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(origen, glm::vec3(0.0f, -3.0f * escala, 3.0f * escala));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 11.0f * escala, 0.3f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pedestal
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 5.74f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 0.5f * escala, 1.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro1
	glBindTexture(GL_TEXTURE_2D, t_micro);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, -0.5f * escala));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 0.3f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2


	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -11.0f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2

	//-----------------------------------------------------------------------------------------
	//---------------------------FIN VOCALISTA------------------------------------------------
	//------------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, 0.0f, -5.0f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, 2.25f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara3);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, -2.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.2f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(3.35f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	//modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(26.5f * escala, 1.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 1.75f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(33.5f * escala, 1.5f * escala, -5.5f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	//modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -1.0f * escala, -0.5f * escala));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 5.0f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	bateria = modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -2.5f * escala, 5.0f * escala));
	bateria = glm::translate(model, glm::vec3(-7.5f, 3.5625f, 0.0f));
	model = glm::rotate(bateria, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f * escala, 1.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria1
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(-2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria3

	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	//-----------------------------------------------------------------------
	//-------------------- Fin baterista --------------------------------------
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);

	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(45.0f * escala, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);

	modelTemp = model = glm::translate(modelaux, glm::vec3(45.0f * escala, 2.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara4);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	guitarra = cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(45.0f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------



	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(41.5f * escala, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelaux, glm::vec3(48.5f * escala, 1.5f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	//guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f, -2.25f, 0.25f));
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der
	//---------------------------------------------------------------


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3
	//-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_gui);
	modelTemp = model = glm::translate(guitarra, glm::vec3(1.2f * escala, 0.7f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(-8.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 2.5f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1
	//---------------------------
	glBindTexture(GL_TEXTURE_2D, t_mango);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.2f * escala, 4.75f * escala, 0.0f));
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f * escala, 7.0f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1

	glBindTexture(GL_TEXTURE_2D, t_play);
	//------------------------------ inicio banda  ------------------------------------------------
	model = glm::translate(modelaux, glm::vec3(26.5f, 3.5625f, 0.0f));
	origen = model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	//model = glm::scale(model, glm::vec3(1.0f*escala, 1.0f*escala, 1.0f*escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);

	modelTemp = model = glm::translate(modelaux, glm::vec3(0.0f * escala, 2.25f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_car);
	model = glm::translate(modelTemp, glm::vec3(0.0f * escala, 1.0f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	guitarra = cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(0.0f * escala, -2.5f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	guitarra = model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der
	glBindTexture(GL_TEXTURE_2D, t_pies);
	//------------------------------------------------------------------------
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(-3.5f * escala, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));

	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	modelTemp = model = glm::translate(modelaux, glm::vec3(3.5f * escala, 1.5f * escala, 0.5f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	//modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	//guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f, -2.25f, 0.25f));
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, 0.25f * escala));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3
	//-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_gui);
	modelTemp = model = glm::translate(guitarra, glm::vec3(2.5f * escala, 2.7f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, glm::vec3(2.5f * escala, 2.5f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1
	//---------------------------
	glBindTexture(GL_TEXTURE_2D, t_mango);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.2f * escala, 4.75f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f * escala, 7.0f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1

	//-----------------------------------------------------------------------------------------
	//							FIN DEL GUITARRISTA
	//-----------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala, 2.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara2);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der
	glBindTexture(GL_TEXTURE_2D, t_pies);
	//------------------------------------------------------------------------
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(6.5f * escala, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(13.5f * escala, 1.5f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der1


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(origen, glm::vec3(0.0f, -3.0f * escala, 3.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 11.0f * escala, 0.3f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pedestal
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 5.74f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 0.5f * escala, 1.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro1
	glBindTexture(GL_TEXTURE_2D, t_micro);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, -0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 0.3f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2


	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -11.0f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2

	//-----------------------------------------------------------------------------------------
	//---------------------------FIN VOCALISTA------------------------------------------------
	//------------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala, 0.0f, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala, 2.25f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara3);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala, -2.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.2f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(3.35f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	//modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(18.5f * escala, 1.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 1.75f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(25.5f * escala, 1.5f * escala, -5.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	//modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -1.0f * escala, -0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 5.0f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	bateria = modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -2.5f * escala, 5.0f * escala));
	bateria = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(bateria, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f * escala, 1.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria1
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(-2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria3

	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	//-----------------------------------------------------------------------
	//-------------------- Fin baterista --------------------------------------
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);

	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, 0.0f, 3.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);

	modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, 2.25f * escala, 3.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara4);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	guitarra = cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala, -2.5f * escala, 3.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------



	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(26.5f * escala, 1.5f * escala, 3.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelaux, glm::vec3(33.5f * escala, 1.5f * escala, 3.0f + 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	//guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f, -2.25f, 0.25f));
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der
	//---------------------------------------------------------------


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3
	//-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_gui);
	modelTemp = model = glm::translate(guitarra, glm::vec3(1.2f * escala, 0.7f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 2.5f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1
	//---------------------------
	glBindTexture(GL_TEXTURE_2D, t_mango);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.2f * escala, 4.75f * escala, 0.0f));
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f * escala, 7.0f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1






	modelaux3 = glm::translate(modelaux, glm::vec3(7.0f, 0.75f, -2.0f));
	for (int i = 0; i < 10; i++) {
		glBindTexture(GL_TEXTURE_2D, t_azul);
		modelaux3 = glm::translate(modelaux3, glm::vec3(0.0f, 0.0f, (-2.0f)));
		model = glm::scale(modelaux3, glm::vec3(1.5f, 3.5f, 1.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_bano);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_bano);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	}
	modelaux3 = glm::translate(modelaux, glm::vec3(1.0f, 00.75f, -2.0f));
	for (int i = 0; i < 10; i++) {
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		modelaux3 = glm::translate(modelaux3, glm::vec3(0.0f, 0.0f, (-2.0f)));
		model = glm::scale(modelaux3, glm::vec3(1.5f, 3.5f, 1.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_bano);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	}
	modelaux2 = model = glm::translate(modelaux, glm::vec3(00.0f, -0.9f, 35.0f));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	//modelaux3 = model = glm::translate(modelaux, glm::vec3(32.0f, -1.74f, 0.0f));
	model = glm::scale(modelaux2, glm::vec3(20.0f, 0.0f, 16.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(414 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(-10.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 4.0f, 16.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_nara);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));





	model = glm::translate(modelaux2, glm::vec3(10.0f, 2.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.25f, 4.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(10.0f, 2.0f, -5.0f));
	model = glm::scale(model, glm::vec3(0.25f, 4.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(0.0f, 2.0f, 7.875f));
	model = glm::scale(model, glm::vec3(19.75f, 4.0f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(0.0f, 2.0f, -7.875f));
	model = glm::scale(model, glm::vec3(19.75f, 4.0f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

	modelaux2 = model = glm::translate(modelaux2, glm::vec3(30.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	modelaux3 = model = glm::translate(modelaux2, glm::vec3(-5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	modelaux4 = model = glm::translate(modelaux2, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f + (0.866f / 4.0f) * 4 + .125, 5.75f));
	model = glm::scale(model, glm::vec3(12.0f, 2.0f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));

	model = glm::translate(modelaux3, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//---------------------------------------------------------

	//Copiame Jair
	/////
	//
	//
	glBindTexture(GL_TEXTURE_2D, t_carp);
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(30.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
	modelaux3 = model = glm::translate(modelaux2, glm::vec3(-5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
	modelaux4 = model = glm::translate(modelaux2, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f + (0.866f / 4.0f) * 4 + .125, 5.75f));
	model = glm::scale(model, glm::vec3(12.0f, 2.0f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));

	model = glm::translate(modelaux3, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	//glBindTexture(GL_TEXTURE_2D, t_metalli);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));

	//Hasta aqui


	//------------------------



	modelaux2 = model = glm::translate(modelaux, glm::vec3(-7.75f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	modelaux2 = model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 6.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));


	model = glm::translate(modelaux2, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	modelaux2 = model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 3.75f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	modelaux2 = model = glm::translate(modelaux, glm::vec3(7.75f, 5.0f, 0.0f));
	model = glm::rotate(modelaux2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelaux2 = model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 6.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(0.0f, 3.0f, 0.0f));

	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelaux2 = model = glm::translate(model, glm::vec3(0.0f, 1.875f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 3.75f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(348 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, t_lego);
	modelaux2 = model = glm::translate(modelaux, glm::vec3(0.0f, 5.0f, -2.75f));
	model = glm::scale(model, glm::vec3(15.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(324 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	modelaux2 = model = glm::translate(modelaux2, glm::vec3(0.0f, 6.0f, 0.0f));
	model = glm::scale(model, glm::vec3(16.0f, 4.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(228 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void *)(234 * sizeof(float)));
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(0.0f, 2.35f, 0.0f));
	model = glm::scale(model, glm::vec3(7.0f, 0.625f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(267 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(270 * sizeof(float)));
	//Carpa
	glBindTexture(GL_TEXTURE_2D, t_carp);
	modelaux2 = model = glm::translate(modelaux, glm::vec3(0.0f, 3.0f, -60.0f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	modelaux3 = model = glm::translate(modelaux2, glm::vec3(-5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	modelaux4 = model = glm::translate(modelaux2, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f + (0.866f / 2.0f) * 4 + 0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));
	model = glm::translate(modelaux3, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	modelaux2 = model = glm::translate(modelaux3, glm::vec3(5.75f, 0.0f, 5.75f));
	model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 4.0f + (0.866f / 2.0f) * 4 + 0.3, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));
	model = glm::translate(modelaux4, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 8.0f, 11.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	//Muchas carpas
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(-16.0f, -2.0f, -30.0f));
	for (int i = 0; i < 3; i++) {
		modelaux2 = model = glm::translate(modelaux2, glm::vec3(8.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		modelaux3 = model = glm::translate(modelaux2, glm::vec3(-2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		modelaux4 = model = glm::translate(modelaux2, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = glm::translate(modelaux2, glm::vec3(0.0f, 1.0f + (0.866f / 2.0f) * 4 + 0.25f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 5.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));

		model = glm::translate(modelaux3, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));


	}
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(16.0f, 0.0f, 0.0f));
	for (int i = 0; i < 3; i++) {
		modelaux2 = model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 8.0f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));

		modelaux3 = model = glm::translate(modelaux2, glm::vec3(-2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		modelaux4 = model = glm::translate(modelaux2, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = glm::translate(modelaux2, glm::vec3(0.0f, 1.0f + (0.866f / 2.0f) * 4 + 0.25f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 5.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));

		model = glm::translate(modelaux3, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	}
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(-40.0f, 0.0f, 8.0f));
	for (int i = 0; i < 3; i++) {
		modelaux2 = model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, -8.0f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		modelaux3 = model = glm::translate(modelaux2, glm::vec3(-2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		modelaux4 = model = glm::translate(modelaux2, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		model = glm::translate(modelaux2, glm::vec3(0.0f, 1.0f + (0.866f / 2.0f) * 4 + 0.25f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 5.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));

		model = glm::translate(modelaux3, glm::vec3(2.75f, 0.0f, 2.25f));
		model = glm::scale(model, glm::vec3(6.0f, 4.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	}
	//termina carpas

	//Otro escenario
	glBindTexture(GL_TEXTURE_2D, t_lego_amarilla);
	modelaux2 = model = glm::translate(modelaux, glm::vec3(31.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 2.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(288 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(306 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
	modelaux3 = model = glm::translate(modelaux2, glm::vec3(5.0f, 0.75f, -4.0f));
	for (int i = 0; i < 10; i++) {
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		modelaux3 = glm::translate(modelaux3, glm::vec3(0.0f, 0.0f, (-2.0f)));
		model = glm::scale(modelaux3, glm::vec3(1.5f, 3.5f, 1.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_bano);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	}
	glBindTexture(GL_TEXTURE_2D, t_agua);
	modelaux3 = model = glm::translate(modelaux3, glm::vec3(32.0f, -1.74f, 0.0f));
	model = glm::scale(modelaux3, glm::vec3(16.0f, 0.0f, 12.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(414 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	//Techo
	glBindTexture(GL_TEXTURE_2D, t_techo);
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(0.0f, 10.0f, 0.0f));
	model = glm::scale(model, glm::vec3(16.0f, 2.0f, 8.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(264 * sizeof(float)));
	//Pared
	model = glm::translate(modelaux2, glm::vec3(0.0f, -6.0f, -4.26f));
	model = glm::scale(model, glm::vec3(12.0f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_lego_amarilla);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(354 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(372 * sizeof(float)));


	//Columnas
	glBindTexture(GL_TEXTURE_2D, t_poste);
	model = glm::translate(modelaux2, glm::vec3(-7.5f, -11.0f, 3.5f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(-7.5f, -11.0f, -3.5f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(+7.5f, -11.0f, -3.5f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(+7.5f, -11.0f, 3.5f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	//Plano de la tierra
	glBindTexture(GL_TEXTURE_2D, t_grande);
	modelaux2 = model = glm::translate(modelaux, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(300.0f, 0.0f, 300.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(414 * sizeof(float)));
	//CAJA
	model = glm::translate(modelaux2, glm::vec3(10.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(200.0f, 100.0f, 200.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cielo);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void *)(378 * sizeof(float)));
	//Columna izq de la entrada
	glBindTexture(GL_TEXTURE_2D, t_hastas);
	modelaux2 = modelaux = model = glm::translate(modelaux, glm::vec3(-38.0f, -1.0f, 30.0f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));//BASE
	//Barricada
	glBindTexture(GL_TEXTURE_2D, t_metal);
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Segunda barricada
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Tercer Barricada
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Cuarta Barricada
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Quinta barricada
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Sexta barricada
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.5f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::rotate(modelaux2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.125f, -2.875f));
	model = glm::scale(model, glm::vec3(0.125f, 12.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 2.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 4.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 8.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.125f, 2.375f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	model = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.125f, 3.0f, 0.125f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));
	//Columna der de la entrada
	glBindTexture(GL_TEXTURE_2D, t_hastas);
	model = glm::translate(modelaux, glm::vec3(16.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, (void *)(60 * sizeof(float)));
	//Tablero de la entrada
	glBindTexture(GL_TEXTURE_2D, t_bandera);
	model = glm::translate(modelaux, glm::vec3(8.0f, 8.5f, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 3.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_metalli);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glBindTexture(GL_TEXTURE_2D, t_toalla);
	//Segundo escenario
	glBindTexture(GL_TEXTURE_2D, t_lego);
	modelaux3 = modelaux2 = model = glm::translate(modelaux, glm::vec3(-18.5f, 1.0f, -13.5f));
	model = glm::scale(model, glm::vec3(16.0f, 2.0f, 6.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(426 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(432 * sizeof(float)));
	modelaux4 = modelaux2 = model = glm::translate(modelaux2, glm::vec3(-13.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 2.0f, 16.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(426 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void *)(432 * sizeof(float)));
	modelaux2 = model = glm::translate(modelaux2, glm::vec3(-4.0f, 9.0f, 0.0f));
	modelaux = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::scale(model, glm::vec3(2.0f, 16.0f, 16.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(426 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(444 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(330 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(336 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(342 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	modelaux2 = model = glm::translate(modelaux2, glm::vec3(5.0f, 7.0f, 0.0f));
	modelaux = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 2.0f, 16.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_lego);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(426 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_madera);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(432 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

	model = glm::translate(modelaux4, glm::vec3(10.0f*escala, 0.0f, 0.0f));
	//-----------------------------------------------------------------------------------------
	//							FIN DEL GUITARRISTA
	//-----------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelaux = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala - 29, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala - 29.0f, 2.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara2);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(10.0f * escala - 29.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der
	glBindTexture(GL_TEXTURE_2D, t_pies);
	//------------------------------------------------------------------------
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(6.5f * escala - 29.0f, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(13.5f * escala - 29.0f, 1.5f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der1


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(origen, glm::vec3(0.0f, -3.0f * escala, 3.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 11.0f * escala, 0.3f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pedestal
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 5.74f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 0.5f * escala, 1.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro1
	glBindTexture(GL_TEXTURE_2D, t_micro);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, -0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 0.3f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2


	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -11.0f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //micro2

	//-----------------------------------------------------------------------------------------
	//---------------------------FIN VOCALISTA------------------------------------------------
	//------------------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala - 39.0f, 0.0f, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);
	modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala - 39.0f, 2.25f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara3);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(22.0f * escala - 39.0f, -2.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------

	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.2f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(3.35f * escala, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	//modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(0.60f * escala, -2.73f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(modelaux, glm::vec3(18.5f * escala - 39.0f, 1.5f * escala, -5.0f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 1.75f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq 
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_play);
	model = glm::translate(modelaux, glm::vec3(25.5f * escala - 39.0f, 1.5f * escala, -5.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, -0.25f * escala));
	//modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	glBindTexture(GL_TEXTURE_2D, t_play);
	modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -1.0f * escala, -0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 0.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f * escala, 5.0f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 0.5f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //banco2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	bateria = modelTemp = model = glm::translate(cinturon, glm::vec3(0.0f, -2.5f * escala, 5.0f * escala));
	bateria = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(bateria, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f * escala, 1.5f * escala, 3.0f * escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria1
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_pede);
	modelTemp = model = glm::translate(bateria, glm::vec3(-2.5f * escala, 2.75f * escala, 0.75f * escala));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f * escala, 2.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //bateria2
	glBindTexture(GL_TEXTURE_2D, t_tan);
	model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, 0.5f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-5.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 0.8f * escala, 2.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//glDrawArrays(GL_QUADS, 190, 24); //bateria3

	glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, (void*)(108 * sizeof(float)));
	glBindTexture(GL_TEXTURE_2D, t_bom);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(492 * sizeof(float)));
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(420 * sizeof(float)));

	//-----------------------------------------------------------------------
	//-------------------- Fin baterista --------------------------------------
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);

	origen = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala - 30.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 4.0f * escala, 0.6f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pecho

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cue);

	modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala - 30.0f, 2.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 0.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cuello

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_cara4);
	model = glm::translate(modelTemp, glm::vec3(0.0f, 1.0f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 2.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 214, 24); //cabeza

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	guitarra = cinturon = modelTemp = model = glm::translate(modelaux, glm::vec3(30.0f * escala - 30.0f, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f * escala, 1.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //cinturon

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(-1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna izq

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(-2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie izq


	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelTemp, glm::vec3(1.75f * escala, -2.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f * escala, 4.0f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pierna der

	//------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pies);
	model = glm::translate(modelTemp, glm::vec3(2.25f * escala, -5.25f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 1.5f * escala, 1.0f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //pie der


	////------------------------------------------------------------------------



	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	modelTemp = model = glm::translate(modelaux, glm::vec3(26.5f * escala - 30.0f, 1.5f * escala, 0.0f));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo izq
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	manoIzq = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.25f * escala, 0.0f));
	//model = glm::translate(model, glm::vec3(-6.5f, 3.5625f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	manoIzq = modelTemp = model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano izq
	////------------------------------------------------------------------------


	////------------------------------------------------------------------------

	//modelTemp = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, t_pantalon);
	model = glm::translate(modelaux, glm::vec3(33.5f * escala - 30.0f, 1.5f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(rasgeo), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f * escala, 1.0f * escala, 0.5f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //brazo der
	////------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_brat);
	//guitarra = modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f, -2.25f, 0.25f));
	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.5f * escala, -2.25f * escala, 0.25f * escala));
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f * escala, 3.5f * escala, 0.3f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //mano der
	//---------------------------------------------------------------


	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 4

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 5
	//---------------------------------------

	modelTemp = model = glm::translate(manoIzq, glm::vec3(-0.5f * escala, -2.0f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f * escala, 0.5f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 1

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 2

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 1.3f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.23f * escala, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.8f * escala, 0.25f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3

	modelTemp = model = glm::translate(modelTemp, glm::vec3(0.25f * escala, 0.15f * escala, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f * escala, 0.18f * escala, 0.25f * escala));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 190, 24); //dedo 3
	//-----------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, t_gui);
	modelTemp = model = glm::translate(guitarra, glm::vec3(1.2f * escala, 0.7f * escala, 0.5f * escala));
	model = glm::translate(model, glm::vec3(26.5f, 3.5625f, 0.0f));
	modelTemp = model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.5f * escala, 2.5f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1
	//---------------------------
	glBindTexture(GL_TEXTURE_2D, t_mango);
	modelTemp = model = glm::translate(modelTemp, glm::vec3(-0.2f * escala, 4.75f * escala, 0.0f));
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f * escala, 7.0f * escala, 0.2f * escala));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 238, 24); //guit 1


	for (int i = 0; i < 2; i++) {
		modelaux2 = model = glm::translate(modelaux3, glm::vec3(-12.0f, 1.0f, -40.0f - (i * 10)));
		model = glm::scale(model, glm::vec3(0.25f, 4.0f, 8.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, t_metalli);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		modelaux4 = model = glm::translate(modelaux2, glm::vec3(4.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 8.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));

		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_metalli);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, t_rolli);
		}

		model = glm::translate(modelaux2, glm::vec3(2.0f, 0.0f, -3.875f));
		model = glm::scale(model, glm::vec3(3.75f, 4.0f, 0.25f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux2, glm::vec3(2.0f, 0.0f, +3.875f));
		model = glm::scale(model, glm::vec3(3.75f, 4.0f, 0.25f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glBindTexture(GL_TEXTURE_2D, t_bandera);
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_queen);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, t_bandera);
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(1.375f, 1.0625f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.125f, 7.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));


		glBindTexture(GL_TEXTURE_2D, t_azul);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 2.0f, -3.75f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glBindTexture(GL_TEXTURE_2D, t_bandera);
		//glBindTexture(GL_TEXTURE_2D, t_bandera);
		glBindTexture(GL_TEXTURE_2D, t_nara);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(480 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));

		//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 2.0f, 3.75f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);


		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(480 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(-2.0f, 2.875f, 0.0f));
		model = glm::scale(model, glm::vec3(3.75f, 0.25f, 7.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_bandera);
		glBindTexture(GL_TEXTURE_2D, t_bandera);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 3.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 7.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_tacos);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, t_pizza);

		}

		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));
	}
	modelaux = modelaux2 = glm::rotate(modelaux2, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelaux3 = modelaux2 = glm::translate(modelaux2, glm::vec3(-13.0f, 0.0f, 0.0f));

	for (int i = 0; i < 3; i++) {
		modelaux2 = glm::translate(modelaux2, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::scale(modelaux2, glm::vec3(0.25f, 4.0f, 8.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

		modelaux4 = model = glm::translate(modelaux2, glm::vec3(4.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 8.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));


		model = glm::translate(modelaux2, glm::vec3(2.0f, 0.0f, -3.875f));
		model = glm::scale(model, glm::vec3(3.75f, 4.0f, 0.25f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glBindTexture(GL_TEXTURE_2D, t_bandera);
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_iron);
		}
		else
		{
			if (i == 1) {
				glBindTexture(GL_TEXTURE_2D, t_beat);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, t_radio);
			}
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux2, glm::vec3(2.0f, 0.0f, +3.875f));
		model = glm::scale(model, glm::vec3(3.75f, 4.0f, 0.25f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//glBindTexture(GL_TEXTURE_2D, t_bandera);
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_gus);
		}
		else
		{
			if (i == 1) {
				glBindTexture(GL_TEXTURE_2D, t_ac);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, t_pink);
			}
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(1.375f, 1.0625f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.125f, 7.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));


		glBindTexture(GL_TEXTURE_2D, t_azul);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 2.0f, -3.75f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		//shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(480 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 2.0f, 3.75f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 0.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, t_nara);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(480 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(-2.0f, 2.875f, 0.0f));
		model = glm::scale(model, glm::vec3(3.75f, 0.25f, 7.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_bandera);
		glBindTexture(GL_TEXTURE_2D, t_bandera);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(float)));

		model = glm::translate(modelaux4, glm::vec3(0.0f, 3.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.0f, 7.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_bandera);
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, t_kaw);
		}
		else {
			if (i == 1) {
				glBindTexture(GL_TEXTURE_2D, t_sushi);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, t_cerv);
			}

		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
		//glBindTexture(GL_TEXTURE_2D, t_metalli);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	}
	modelaux3 = glm::translate(modelaux3, glm::vec3(-10.0f, -0.25f, -16.0f));
	for (int i = 0; i < 10; i++) {
		modelaux3 = glm::translate(modelaux3, glm::vec3(0.0f, 0.0f, (-2.0f)));
		model = glm::scale(modelaux3, glm::vec3(1.5f, 3.5f, 1.5f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
		glBindTexture(GL_TEXTURE_2D, t_bano);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
		shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
	}
	modelaux2 = glm::rotate(modelaux, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelaux2 = glm::translate(modelaux2, glm::vec3(6.5f, -0.875f, 0.0f));
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 5; i++) {
			modelaux2 = glm::translate(modelaux2, glm::vec3(6.0f, -0.0f, 0.0f));
			model = glm::scale(modelaux2, glm::vec3(2.0f, 0.25f, 3.0f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, t_azul);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(468 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(474 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)(24 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			model = glm::translate(modelaux2, glm::vec3(0.0f, -0.5f, -1.625f));
			model = glm::scale(model, glm::vec3(2.0f, 1.25f, 0.25f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, t_azul);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(486 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			model = glm::translate(modelaux2, glm::vec3(0.0f, -0.5f, +1.625f));
			model = glm::scale(model, glm::vec3(2.0f, 1.25f, 0.25f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_azul);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(486 * sizeof(float)));

			modelaux3 = model = glm::translate(modelaux2, glm::vec3(1.5f, -0.5f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.25f, 2.0f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

			model = glm::translate(modelaux3, glm::vec3(0.0f, -0.25f, 1.0625f));
			model = glm::scale(model, glm::vec3(1.0f, 0.75f, 0.125f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_nara);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));

			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

			model = glm::translate(modelaux3, glm::vec3(0.0f, -0.25f, -1.0625f));
			model = glm::scale(model, glm::vec3(1.0f, 0.75f, 0.125f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			 //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_nara);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

			modelaux3 = model = glm::translate(modelaux2, glm::vec3(-1.5f, -0.5f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 0.25f, 2.0f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

			model = glm::translate(modelaux3, glm::vec3(0.0f, -0.25f, 1.0625f));
			model = glm::scale(model, glm::vec3(1.0f, 0.75f, 0.125f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_nara);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));

			model = glm::translate(modelaux3, glm::vec3(0.0f, -0.25f, -1.0625f));
			model = glm::scale(model, glm::vec3(1.0f, 0.75f, 0.125f));
			shader.setMat4("model", model);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(318 * sizeof(float)));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(312 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_nara);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(360 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(366 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(456 * sizeof(float)));
			glBindTexture(GL_TEXTURE_2D, t_metalli);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(462 * sizeof(float)));
			shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(float)));
		}

		modelaux2 = glm::translate(modelaux2, glm::vec3(-30.0f, -0.0f, 4.0f));
	}

	// ****************** Personaje 1 ************************ //
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 34, posY, posZ -60.8));
	tmp = model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//********************** Fin Personaje 1 *********************//
	
	// ******************* Personaje 2 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(23,-18, 6));
	model = glm::translate(model, glm::vec3(posX - 35.7, posY, posZ - 57.8));
	tmp = model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Personaje 2 ************************//

	// ******************* Personaje 3 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 39.26, posY, posZ - 51.5));
	tmp = model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 3 ************************//

	// ******************* Personaje 4 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 40.8, posY, posZ - 49));
	tmp = model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 4 ************************//

	// ******************* Personaje 5 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 43, posY, posZ - 44.5));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego5.Draw(shader);
	//******************* Fin Personaje 5 ************************//

	// ******************* Personaje 6 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 44.85, posY, posZ - 41));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 6 ************************//

	// ******************* Personaje 7 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 44, posY, posZ - 38));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 7 ************************//

	// ******************* Personaje 8 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 41, posY, posZ - 36));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-100.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 8 ************************//

	// ******************* Personaje 9 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 37.5, posY, posZ - 34));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 9 ************************//

	// ******************* Personaje 10 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 34, posY, posZ - 32));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 10 ************************//

	// ******************* Personaje 11 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 31, posY, posZ - 62.5));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 11 ************************//

	// ******************* Personaje 12 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 28, posY, posZ - 60.8));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 12 ************************//

	// ******************* Personaje 13 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 20.6, posY, posZ - 56.4));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 13 ************************//

	// ******************* Personaje 14 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 18, posY, posZ - 54.8));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 14 ************************//


	// ******************* Personaje 15 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 10, posY, posZ - 50.3));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 15 ************************//

	// ******************* Personaje 16 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 7, posY, posZ - 48.6));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 16 ************************//

	// ******************* Personaje 17 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 2, posY, posZ - 46.5));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 17 ************************//

	// ******************* Personaje 18 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(19, -18, 11));
	model = glm::translate(model, glm::vec3(posX + 2, posY, posZ - 44.5));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Fin Personaje 18 ************************//

	// ******************* Personaje 19 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX + 2, posY, posZ - 41));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-140.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego5.Draw(shader);
	//******************* Fin Personaje 19 ************************//

	// ******************* Personaje 20 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX + 1, posY, posZ - 37.5));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-160.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 20 ************************//

	// ******************* Personaje 21 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 1, posY, posZ - 34));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 21 ************************//Ç

	// ******************* Personaje 22 *************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 2.55, posY, posZ - 31));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 22 ************************//

	// ******************* Personaje 23 * ************************//
		model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 0));
	model = glm::translate(model, glm::vec3(posX - 4.5, posY, posZ - 28));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 23 ************************//

	// ******************* Personaje 24 * ************************//
		model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX + 2, posY, posZ + 7));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego5.Draw(shader);
	//******************* Fin Personaje 24 ************************//


	// ******************* Personaje 25 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX + 0, posY, posZ + 8));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 25 ************************//

	// ******************* Personaje 26 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX -5, posY, posZ + 8));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 26 ************************//

	// ******************* Personaje 27 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-18 + monitoX, -18 + monitoY, 24));
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Fin Personaje 27 ************************//

	// ******************* Personaje 28 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX - 5, posY, posZ + 12));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 28 ************************//

	// ******************* Personaje 29 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX - 5, posY, posZ + 7));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 29 ************************//

	// ******************* Personaje 30 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX -10, posY, posZ + 15));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono3), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 30 ************************//

	// ******************* Personaje 31 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX - 10, posY, posZ + 12));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono3), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 31 ************************//

	// ******************* Personaje 32 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX - 11, posY, posZ + 5));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono3), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 32 ************************//

	// ******************* Personaje 33 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX - 10, posY, posZ + 8));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono3), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 33 ************************//

	// ******************* Personaje 34 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX +2.5, posY, posZ + 12));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono3), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 34 ************************//

	// ******************* Personaje 35 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0 + monitoX, 1.2 + monitoY, 0));
	model = glm::translate(model, glm::vec3(posX +4, posY, posZ + 10));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 35 ************************//

	// ----------------- Escenario 2 ------------------------------//
	// ******************* Personaje 36 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -10));
	model = glm::translate(model, glm::vec3(posX - 65, posY, posZ ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 36 ************************//

	// ******************* Personaje 37 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12 + monitoX, -18 + monitoY, 36));
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Fin Personaje 37 ************************//

	// ******************* Personaje 38 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -12));
	model = glm::translate(model, glm::vec3(posX - 62, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 38 ************************//

	// ******************* Personaje 38 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -10));
	model = glm::translate(model, glm::vec3(posX - 62, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 38 ************************//

	// ******************* Personaje 39 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 60, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 39 ************************//

	// ******************* Personaje 40 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -9));
	model = glm::translate(model, glm::vec3(posX - 60, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 40 ************************//

	// ******************* Personaje 41 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-13 + monitoX, -18 + monitoY, 39));
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Fin Personaje 41 ************************//
	
	// ******************* Personaje 42 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 57, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego3.Draw(shader);
	//******************* Fin Personaje 42 ************************//

	// ******************* Personaje 43 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -4));
	model = glm::translate(model, glm::vec3(posX - 54, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 43 ************************//

	// ******************* Personaje 44 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 54, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego5.Draw(shader);
	//******************* Fin Personaje 44 ************************//

	// ******************* Personaje 45 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -4));
	model = glm::translate(model, glm::vec3(posX - 52, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 45 ************************//

	// ******************* Personaje 46 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 50, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 46 ************************//

	// ******************* Personaje 47 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -4));
	model = glm::translate(model, glm::vec3(posX - 50, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 47 ************************//

	// ******************* Personaje 48 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -8));
	model = glm::translate(model, glm::vec3(posX - 68, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 48 ************************//

	// ******************* Personaje 49 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -8));
	model = glm::translate(model, glm::vec3(posX - 65, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego5.Draw(shader);
	//******************* Fin Personaje 49 ************************//

	// ******************* Personaje 49 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -6));
	model = glm::translate(model, glm::vec3(posX - 63, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego7.Draw(shader);
	//******************* Fin Personaje 49 ************************//

	// ******************* Personaje 50 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -4));
	model = glm::translate(model, glm::vec3(posX -59, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 50 ************************//

	// ******************* Personaje 51 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -3));
	model = glm::translate(model, glm::vec3(posX - 57, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego4.Draw(shader);
	//******************* Fin Personaje 51 ************************//

	// ******************* Personaje 52 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -1));
	model = glm::translate(model, glm::vec3(posX - 54, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego6.Draw(shader);
	//******************* Fin Personaje 52 ************************//

	// ******************* Personaje 53 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12 + monitoX, -18 + monitoY, 32));
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego2.Draw(shader);
	//******************* Fin Personaje 53 ************************//

	// ******************* Personaje 54 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -2));
	model = glm::translate(model, glm::vec3(posX - 49, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 54 ************************//

	// ******************* Personaje 55 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -5));
	model = glm::translate(model, glm::vec3(posX - 47, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 55 ************************//

	// ******************* Personaje 56 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 47, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 56 ************************//

	// ******************* Personaje 57 * ************************//
		model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -7));
	model = glm::translate(model, glm::vec3(posX - 44, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 57 ************************//

	// ******************* Personaje 58 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -9));
	model = glm::translate(model, glm::vec3(posX - 48, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 58 ************************//

	// ******************* Personaje 59 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -11));
	model = glm::translate(model, glm::vec3(posX - 48, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 59 ************************//

	// ******************* Personaje 60 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -10));
	model = glm::translate(model, glm::vec3(posX - 45, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 60 ************************//

	// ******************* Personaje 61 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -10.5));
	model = glm::translate(model, glm::vec3(posX - 42, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 61 ************************//

	// ******************* Personaje 62 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -13));
	model = glm::translate(model, glm::vec3(posX - 43, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 62 ************************//

	// ******************* Personaje 63 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -13));
	model = glm::translate(model, glm::vec3(posX - 45, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 63 ************************//

	// ******************* Personaje 64 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -14));
	model = glm::translate(model, glm::vec3(posX - 47, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 64 ************************//

	// ******************* Personaje 65 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -15));
	model = glm::translate(model, glm::vec3(posX - 45, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 65 ************************//

	// ******************* Personaje 66 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -16));
	model = glm::translate(model, glm::vec3(posX - 43, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 66 ************************//

	// ******************* Personaje 67 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -16 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 48, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 67 ************************//

	// ******************* Personaje 68 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -17 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 50, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 68 ************************//

	// ******************* Personaje 69 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -17 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 53, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 69 ************************//

	// ******************* Personaje 70 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -18.5 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 55, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 70 ************************//

	// ******************* Personaje 71 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -19.5 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 58, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 71 ************************//

	// ******************* Personaje 72 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -22 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 59, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 72 ************************//

	// ******************* Personaje 73 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -23 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 56, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 73 ************************//

	// ******************* Personaje 74 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -21.5 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 54, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 74 ************************//


	// ******************* Personaje 75 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2 + caeMonito2, -20.5 - desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 52, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono2), glm::vec3(0.0f, 1.0f, 0.0));
	tmp = model = glm::rotate(model, glm::radians(caeMonito), glm::vec3(0.0f, 0.0f, 1.0));
	//tmp = model = glm::rotate(model, glm::radians(caeMonito2), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 75 ************************//

	// ******************* Personaje 76 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -19.5 + desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 48, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 76 ************************//

	// ******************* Personaje 77 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -19.5 + desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 45, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 77 ************************//

	// ******************* Personaje 78 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -22 + desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 49, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 78 ************************//

	// ******************* Personaje 79 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -24 + desplazaMono));
	model = glm::translate(model, glm::vec3(posX -51, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 79 ************************//

	// ******************* Personaje 80 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, -25 + desplazaMono));
	model = glm::translate(model, glm::vec3(posX - 54, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(movimientoMono), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 80 ************************//

	// ------------------- Entrada -------------------------------//
	// ******************* Personaje 81 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 26));
	model = glm::translate(model, glm::vec3(posX - 44.5, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 81 ************************//

	// ******************* Personaje 82 * ************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.2, 22));
	model = glm::translate(model, glm::vec3(posX - 42, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	lego1.Draw(shader);
	//******************* Fin Personaje 82 ************************//

	//******************* Camarografos ***************************//
	//------------------- Camarografo 1 --------------------------//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, -10, -14));
	model = glm::translate(model, glm::vec3(posX - 75, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	camarografo.Draw(shader);

	//------------------ Camarografo 2 --------------------------//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, -10, -13));
	model = glm::translate(model, glm::vec3(posX - 50, posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	camarografo.Draw(shader);

	//********************** Carro ***************************//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, -1.2, 46));
	model = glm::translate(model, glm::vec3(posX - 23 , posY, posZ));
	tmp = model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
	shader.setMat4("model", model);
	carro.Draw(shader);

	glBindVertexArray(0);
	
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LSVC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, my_input);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	//Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	// Load models



	
	Model lego1 = ((char *)"Models/LegoMan/LegoMan2/LegoMan.obj");
	Model lego2 = ((char *)"Models/Leia/Leia.obj");
	Model lego3 = ((char *)"Models/LegoMan/LegoMan2/LegoMan.obj");
	Model lego4 = ((char *)"Models/LegoMan/LegoMan3/LegoMan.obj");
	Model lego5 = ((char *)"Models/LegoMan/LegoMan3/LegoMan.obj");
	Model lego6 = ((char *)"Models/LegoMan/LegoMan4/LegoMan.obj");
	Model lego7 = ((char *)"Models/LegoMan/LegoMan2/LegoMan.obj");
	Model camarografo = ((char*)"Models/Han/HanSolo.obj");
	Model carro = ((char*)"Models/Carro/mustang_GT.obj");

	//Inicializaci�n de KeyFrames

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	//declaracion de key frames

	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = -2.0f;
	KeyFrame[0].posZ = -40.0f;
	KeyFrame[0].rotRodIzq = 0.0f;
	KeyFrame[0].giroMonito = 0.0f;

	FrameIndex++;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 15.0f;
	KeyFrame[1].posZ = -40.0f;
	KeyFrame[1].rotRodIzq = 1080.0f;
	KeyFrame[1].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[2].posX = 0.0f;
	KeyFrame[2].posY = 15.0f;
	KeyFrame[2].posZ = -60.0f;
	KeyFrame[2].rotRodIzq = 1080.0f;
	KeyFrame[2].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[3].posX = 15.0f;
	KeyFrame[3].posY = 15.0f;
	KeyFrame[3].posZ = -40.0f;
	KeyFrame[3].rotRodIzq = 990.0f;
	KeyFrame[3].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[4].posX = 0.0f;
	KeyFrame[4].posY = 15.0f;
	KeyFrame[4].posZ = -20.0f;
	KeyFrame[4].rotRodIzq = 900.0f;
	KeyFrame[4].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[5].posX = -15.0f;
	KeyFrame[5].posY = 15.0f;
	KeyFrame[5].posZ = -40.0f;
	KeyFrame[5].rotRodIzq = 810.0f;
	KeyFrame[5].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[6].posX = 0.0f;
	KeyFrame[6].posY = 15.0f;
	KeyFrame[6].posZ = -60.0f;
	KeyFrame[6].rotRodIzq = 720.0f;
	KeyFrame[6].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[7].posX = 0.0f;
	KeyFrame[7].posY = 15.0f;
	KeyFrame[7].posZ = -40.0f;
	KeyFrame[7].rotRodIzq = 720.0f;
	KeyFrame[7].giroMonito = 0.08f;

	FrameIndex++;

	KeyFrame[8].posX = 0.0f;
	KeyFrame[8].posY = -2.0f;
	KeyFrame[8].posZ = -40.0f;
	KeyFrame[8].rotRodIzq = -360.0f;
	KeyFrame[8].giroMonito = 0.0f;

	FrameIndex++;

	/*printf("ERROR::ASSIMP:: Unable to open file Models / silla / Silla.obj. \n\n");*/

	for (int i = 0; i < FrameIndex; i++) {
		printf("posX[%i] = %.1f \n", i, KeyFrame[i].posX);
		printf("posY[%i] = %.1f \n", i, KeyFrame[i].posY);
		printf("posZ[%i] = %.1f \n", i, KeyFrame[i].posZ);
		printf("rot[%i] = %.1f  \n", i, KeyFrame[i].rotRodIzq);
		printf("esc[%i] = %.1f \n\n", i, KeyFrame[i].giroMonito);
	}
    
	

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        //my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(modelShader,lego1, lego2, lego3, lego4, lego5, lego6, lego7, camarografo, carro);	
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		noche = false;

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		noche = true;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {

			movimientoMono = -30.0f,
			movimientoMono2 = -30.0f,
			desplazaMono = 0.0f,
			caeMonito = 0.0f,
			caeMonito2 = 0.0f;
			mueveMono = true;
			
	}
		
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {

		
		mueveMono2 = true;

	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		musica = 1;
		sonido();

	}

	//To play KeyFrame animation 

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		toca = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		recorrido = true;

	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}