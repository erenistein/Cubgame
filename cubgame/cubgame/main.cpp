#include <GL/glut.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
using namespace std;

static void Timer(int value)
{
    glutPostRedisplay();

    glutTimerFunc(1, Timer, value);
}

class Cube
{
public:
    float x;
    float y;
    float width;
    float height;
    float speed;

    Cube(float xIn, float yIn, float wIn, float hIn, float sIn)
    {
        x = xIn;
        y = yIn;
        width = wIn;
        height = hIn;
        speed = sIn;
    }

    virtual void draw() = 0;
    virtual void move() {}
};

class Player :public Cube
{
public:
    int score;
    int life;
    int array[4];
    int level;

    Player(float xIn, float yIn, float wIn, float hIn, float sIn) : Cube(xIn, yIn, wIn, hIn, sIn)
    {
        level = 0;
        score = 0;
        life = 3;
        for (int i = 0; i < 4; i++)
        {
            array[i] = 0;
        }
    }

    void draw()
    {
        glColor3f(1.0, 0.25, 1.0);
        glBegin(GL_POLYGON);
        glVertex3f(x, y, 0.0);
        glVertex3f(x + width, y, 0.0);
        glVertex3f(x + width, y + height, 0.0);
        glVertex3f(x, y + height, 0.0);
        glEnd();
        glFlush();
    }

    float moveIn(float coordinate, float speedIn)
    {
        return coordinate + speedIn;
    }

    void move()
    {
        for (int i = 0; i < 4; i++)
        {
            if (array[i] == 1)
            {
                array[i] = 0;

                if (i == 0)
                {
                    x = moveIn(x, -speed);
                }

                if (i == 1)
                {
                    x = moveIn(x, +speed);
                }

                if (i == 2)
                {
                    y = moveIn(y, -speed);
                }

                if (i == 3)
                {
                    y = moveIn(y, +speed);
                }
                if (x <= 0)
                {
                    x = 0;
                }
                if (y <= 0)
                {
                    y = 0;
                }
                if (x + width >= 480)
                {
                    x = 480 - width;
                }
                if (y + height >= 640)
                {
                    x = 215;
                    y = 95;
                    score++;
                    level = 1;
                }
            }
        }
    }

    void collision(float xIn, float wIn, float yIn, float hIn)
    {
        if ((xIn >= x && xIn <= x + width) || (xIn + wIn >= x && xIn + wIn <= x + width))
        {
            if ((yIn >= y && yIn <= y + height) || (yIn + hIn >= y && yIn + hIn <= y + height))
            {
                life--;
                x = 215;
                y = 95;
                if (life <= 0)
                {
                    exit(0);
                }
            }
        }
    }
};

class Enemy :public Cube
{
public:
    float fanLength;
    float fanSpeed;
    float fanX;
    float fanY;
    float rColor;
    float gColor;
    float bColor;

    Enemy(float xIn, float yIn, float wIn, float hIn, float sIn, float rIn, float gIn, float bIn) : Cube(xIn, yIn, wIn, hIn, sIn)
    {
        fanLength = 48;
        fanSpeed = 2;
        fanX = xIn + wIn / 2;
        fanY = yIn + hIn + +2;
        rColor = rIn;
        gColor = gIn;
        bColor = bIn;
    }

    void levelUp()
    {
        speed *= 1.5;
    }

    void move()
    {
        if (speed <= 0)
        {
            if (x >= 0)
            {
                x += speed;
            }
            else {
                speed *= -1;
            }
        }
        else {
            if (x + width <= 480)
            {
                x += speed;
            }
            else {
                speed *= -1;
            }
        }
        fanX = x + width / 2;
    }

    void draw()
    {
        glColor3f(rColor, gColor, bColor);
        glBegin(GL_POLYGON);
        glVertex3f(x, y, 0.0);
        glVertex3f(x + width, y, 0.0);
        glVertex3f(x + width, y + height, 0.0);
        glVertex3f(x, y + height, 0.0);
        glEnd();
        glFlush();

        fanLength += fanSpeed;
        if (fanLength >= 50)
        {
            fanSpeed *= -1;
            fanLength = 48;
        }
        if (fanLength <= 0)
        {
            fanSpeed *= -1;
            fanLength = 2;
        }


        glColor3f(1.0, 0.0, 0.7);
        glBegin(GL_LINES);
        glVertex2f(fanX - fanLength / 2, fanY);
        glVertex2f(fanX + fanLength / 2, fanY);
        glEnd();
        glFlush();
    }
};

shared_ptr<Player> myCube;
vector<shared_ptr<Enemy>> arr;

void keyboard(unsigned char key, int x, int y)
{
    if (key == 97)
    {
        myCube->array[0] = 1;
    }
    if (key == 100)
    {
        myCube->array[1] = 1;
    }
    if (key == 115)
    {
        myCube->array[2] = 1;
    }
    if (key == 119)
    {
        myCube->array[3] = 1;
    }

    glutPostRedisplay();
}


void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.30, 0.55, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0, 0.0);
    glVertex3f(480, 0, 0.0);
    glVertex3f(480, 240, 0.0);
    glVertex3f(0, 240, 0.0);
    glEnd();
    glFlush();

    myCube->move();

    if (myCube->level == 1)
    {
        myCube->level = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            arr[i]->levelUp();
        }
    }



    for (int i = 0; i < arr.size(); i++)
    {
        arr[i]->move();
        arr[i]->draw();
        myCube->collision(arr[i]->x, arr[i]->width, arr[i]->y, arr[i]->height);
    }

    myCube->draw();


    glColor3f(0, 0, 0);
    glRasterPos2f(250, 630);
    char puantext[4] = { 'P','u','a','n' };
    for (int i = 0; i < 4; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puantext[i]);
    }

    char puan = myCube->score + '0';
    glColor3f(1, 0, 0);
    glRasterPos2f(300, 630);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puan);

    glColor3f(0, 0, 0);
    glRasterPos2f(350, 630);
    char lifetext[3] = { 'C','a','n' };
    for (int i = 0; i < 3; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, lifetext[i]);
    }

    char life = myCube->life + '0';
    glColor3f(1, 0, 0);
    glRasterPos2f(400, 630);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, life);

    glutSwapBuffers();

}


void myInit()
{
    glClearColor(0, 1.0, 1.0, 0.0);

    glColor3f(1.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION); 

    glOrtho(0.0, 480.0, 0.0, 640.0, -1.0, 1.0);
}

bool isMouseInsideButton(int x, int y, int buttonWidth, int buttonHeight, int mouseX, int mouseY) {
    return (mouseX >= x && mouseX <= x + buttonWidth && mouseY >= y && mouseY <= y + buttonHeight);
}

void drawButton(int x, int y, int width, int height, string buttonText) // Argüman düzeltildi
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x + width / 2 - (buttonText.size() * 6) / 2, y + height / 2);
    for (int i = 0; i < buttonText.size(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buttonText[i]);
    }
}

enum class GameMode { Mode2D, Mode3D };
GameMode currentMode = GameMode::Mode2D; // Sona eklenen satýr

// 2D mod için render fonksiyonu
void render2D()
{
    drawButton(400, 600, 80, 40, "3D Mode"); // Süslü parantez ve noktalý virgül düzeltildi
}

// 3D mod için render fonksiyonu

void render3D()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Derinlik tamponunu da temizle
    glEnable(GL_DEPTH_TEST); // Derinlik testini etkinleþtir

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0); // Perspektif projeksiyonu ayarla

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Kamera konumunu ayarla

    static float angle = 0.0; // Küpün dönüþ açýsý
    angle += 0.5; // Her çaðrýda açýyý arttýr

    glPushMatrix(); // Küpün dönüþümünü korumak için bir dönüþüm matrisi yýðýný oluþtur

    glRotatef(angle, 0.0, 1.0, 0.0); // Küpü y ekseninde döndür

    glColor3f(1.0, 0.0, 0.0); // Kýrmýzý renkte bir küp çiz
    glutSolidCube(1.0);

    glPopMatrix(); // Dönüþüm matrisini geri al

    glutSwapBuffers();
}



// Mod deðiþimini saðlayan fonksiyonlar
void switchToMode2D() {
    currentMode = GameMode::Mode2D;
    glutDisplayFunc(render2D);
    glutPostRedisplay(); // Ekrana yeniden çizim talebi gönder
}

void switchToMode3D() {
    currentMode = GameMode::Mode3D;
    glutDisplayFunc(render3D);
    glutPostRedisplay(); // Ekrana yeniden çizim talebi gönder
}
void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            switchToMode3D(); // Sol týklama ile 3D'ye geçiþ
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            switchToMode2D(); // Sað týklama ile 2D'ye geçiþ
        break;
    default:
        break;
    }
}



int main(int argc, char** argv) // main fonksiyonunun imzasý düzeltildi
{
    myCube = make_shared<Player>(215, 95, 50, 50, 10);
    arr.push_back(make_shared<Enemy>(0, 250, 50, 50, 0.2, 0, 0, 0));
    arr.push_back(make_shared<Enemy>(430, 320, 50, 50, 0.3, 1, 1, 0));
    arr.push_back(make_shared<Enemy>(430, 400, 50, 50, 0.4, 0, 1, 0));
    arr.push_back(make_shared<Enemy>(0, 470, 50, 50, 0.5, 1, 0, 0));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(480, 640);

    glutInitWindowPosition(0, 0);

    glutCreateWindow("CubGame");

    myInit();

    glutKeyboardFunc(keyboard);

    glutDisplayFunc(render);

    glutMouseFunc(mouse);
    Timer(0);

    glutMainLoop();
}
