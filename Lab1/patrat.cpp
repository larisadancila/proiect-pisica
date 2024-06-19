#include "glos.h"
#include "gl.h"
#include "glaux.h"
#include "glut.h"

static int body = 0;
static int head = 0;
static int tail = 0;
static int spin = 0;

GLuint textureId1;
GLuint incarcaTextura(const char* s)
{
    GLuint textureId = 0;
    AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);
    if (pImagineTextura != NULL)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
    }
    if (pImagineTextura)
    {
        if (pImagineTextura->data) {
            free(pImagineTextura->data);
        }
        free(pImagineTextura);
    }
    return textureId;
}

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK MutaStanga(void);
void CALLBACK MutaDreapta(void);
void CALLBACK MutaSus(void);
void CALLBACK MutaJos(void);
void CALLBACK MutaCoadaS(void);
void CALLBACK MutaCoadaJ(void);
void CALLBACK movelight(AUX_EVENTREC* event);
void drawBezierCurve();
void drawNurbsSurface();
;
void drawText(const char* text, int length, int x, int y);

void CALLBACK MutaStanga(void) {
    body = body - 10;
}

void CALLBACK MutaDreapta(void) {
    body = body + 10;
}
void CALLBACK MutaTotSus(void) {
    body = body - 10;
}

void CALLBACK MutaTotJos(void) {
    body = body + 10;
}

void CALLBACK MutaSus(void) {
    head = head + 10;
}

void CALLBACK MutaJos(void) {
    head = head - 10;
}

void CALLBACK MutaCoadaS(void) {
    tail = (tail + 5) % 360;
}
void CALLBACK RotesteDreapta() {

}
void CALLBACK MutaCoadaJ(void) {
    tail = (tail - 5) % 360;
}
void CALLBACK movelight(AUX_EVENTREC* event) {
    spin = (spin + 30) % 360;
}
void drawBezierCurve() {
    GLfloat ctrlpoints[4][3] = {
    { -4.0, -4.0, 0.0 },
    { -2.0,  4.0, 0.0 },
    {  2.0, -4.0, 0.0 },
    {  4.0,  4.0, 0.0 }
    };

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 30; i++) {
        glEvalCoord1f((GLfloat)i / 30.0);
    }
    glEnd();
}

void drawNurbsSurface() {
    GLUnurbsObj* theNurb;
    theNurb = gluNewNurbsRenderer();
    GLfloat ctlpoints[4][4][3];
    GLfloat knots[8] = { 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0 };
    for (int u = 0; u < 4; u++) {
        for (int v = 0; v < 4; v++) {
            ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5);
            ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5);
            if ((u == 1 || u == 2) && (v == 1 || v == 2))
                ctlpoints[u][v][2] = 3.0;
            else
                ctlpoints[u][v][2] = -3.0;
        }
    }gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);
}

void drawText(const char* text, int length, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void cat() {
    GLUquadricObj* obj;
    obj = gluNewQuadric();
    glPushMatrix();
    glColor3ub(155, 55, 55);
    glTranslatef(0, 0, -25);
    glRotatef(45, 0, 0, 1);
    gluCylinder(obj, 16, 16, 64, 4, 10);
    gluDisk(obj, 0, 16, 4, 10);
    glTranslatef(0, 0, 64);
    gluDisk(obj, 0, 16, 4, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(5, 55, 55);
    glTranslatef(0, 6, 37);
    glRotatef((GLfloat)head, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    gluCylinder(obj, 17, 17, 20, 4, 2);
    gluDisk(obj, 0, 17, 4, 10);
    glTranslatef(0, 0, 20);
    gluDisk(obj, 0, 17, 4, 10);


    glRotatef(90, 1, 1, 0);
    glTranslatef(-3, -3, -7.5);
    gluCylinder(obj, 10, 10, 15, 4, 2);
    gluDisk(obj, 0, 10, 4, 10);
    glTranslatef(0, 0, 15);
    gluDisk(obj, 0, 10, 4, 10);


    glRotatef(-90, 1, 1, 0);
    glTranslatef(10, 13, -20);
    gluCylinder(obj, 4, 4, 8, 4, 2);
    gluDisk(obj, 0, 4, 4, 10);
    glTranslatef(0, 0, 8);
    gluDisk(obj, 0, 4, 4, 10);
    glTranslatef(-8, 8, -8);
    gluCylinder(obj, 4, 4, 8, 4, 2);
    gluDisk(obj, 0, 4, 4, 10);
    glTranslatef(0, 0, 8);
    gluDisk(obj, 0, 4, 4, 10);
    glPopMatrix();


    glPushMatrix();
    glColor3ub(55, 5, 55);
    glTranslatef(0, 8, -24);
    glRotatef((GLfloat)tail, 1, 0, 0);
    glTranslatef(0, -8, 24);
    glTranslatef(0, 8, -56);
    glRotatef(45, 0, 0, 1);
    gluCylinder(obj, 4, 4, 32, 4, 10);
    gluDisk(obj, 0, 4, 4, 10);
    glTranslatef(0, 0, 32);
    gluDisk(obj, 0, 4, 4, 10);


    glRotatef(90, 0, 0, 1);
    glTranslatef(0, 0, -30);
    glRotatef((GLfloat)tail, 1, 1, 0);
    glTranslatef(0, 0, 30);
    glTranslatef(0, 0, -60);
    gluCylinder(obj, 4, 4, 32, 4, 10);
    gluDisk(obj, 0, 4, 4, 10);
    glTranslatef(0, 0, 32);
    gluDisk(obj, 0, 4, 4, 10);
    glPopMatrix();


    glPushMatrix();
    glColor3ub(5, 55, 105);
    glRotatef(135, 0, 0, 1);
    glTranslatef(-15.5, 24.5, 24);
    glRotatef(90, 1, 1, 0);
    gluCylinder(obj, 8, 8, 40, 4, 10);
    gluDisk(obj, 0, 8, 4, 10);
    glTranslatef(0, 0, 40);
    gluDisk(obj, 0, 8, 4, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(105, 105, 55);
    glRotatef(135, 0, 0, 1);
    glTranslatef(-15.5, 24.5, -14);
    glRotatef(90, 1, 1, 0);
    gluCylinder(obj, 8, 8, 24, 4, 10);
    gluDisk(obj, 0, 8, 4, 10);
    glTranslatef(0, 0, 24);
    gluDisk(obj, 0, 8, 4, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(5, 55, 105);
    glRotatef(135, 0, 0, 1);
    glTranslatef(-24.5, 15.5, 24);
    glRotatef(90, 1, 1, 0);
    gluCylinder(obj, 8, 8, 40, 4, 10);
    gluDisk(obj, 0, 8, 4, 10);
    glTranslatef(0, 0, 40);
    gluDisk(obj, 0, 8, 4, 10);
    glPopMatrix();


    glPushMatrix();
    glColor3ub(105, 105, 55);
    glRotatef(135, 0, 0, 1);
    glTranslatef(-24.5, 15.5, -14);
    glRotatef(90, 1, 1, 0);
    gluCylinder(obj, 8, 8, 24, 4, 10);
    gluDisk(obj, 0, 8, 4, 10);
    glTranslatef(0, 0, 24);
    gluDisk(obj, 0, 8, 4, 10);
    glPopMatrix();

    gluQuadricTexture(obj, GL_FALSE);


    glBindTexture(GL_TEXTURE_2D, 0);
}

void myinit(void) {
    glEnable(GL_MAP1_VERTEX_3);
    textureId1 = incarcaTextura("covor.bmp");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 95.0 };

    GLfloat light_ambient[] = { 0, 0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };

    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

}
void drawString(const char* str, int x, int y) {
    glRasterPos2i(x, y);
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void CALLBACK display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glColor3f(1.0, 1.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);

    GLfloat position[] = { 0.0, 2, 2, 0.0 };
    glPushMatrix();
    glRotated((GLdouble)spin, 0.0, 1.0, 0.0);
    glRotated(0.0, 1.0, 0.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glTranslated(0.0, 0.0, 70);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 1.0);
    auxWireCube(10);
    glEnable(GL_LIGHTING);

    glPopMatrix();

    glRotatef((GLfloat)body, 0, 1, 0);
    cat();

    drawBezierCurve();
    drawNurbsSurface();



    glTranslatef(-25.0, -50.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    drawString("Dancila Larisa-Maria", 0, 0);

    glPopMatrix();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-60.0, 60.0, -60.0 * (GLfloat)h / (GLfloat)w,
            60.0 * (GLfloat)h / (GLfloat)w, -100.0, 100.0);
    else
        glOrtho(-60.0 * (GLfloat)w / (GLfloat)h,
            60.0 * (GLfloat)w / (GLfloat)h, -60.0, 60.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
    auxInitPosition(0, 0, 1200, 800);
    auxInitWindow("Cat");
    myinit();
    auxKeyFunc(AUX_LEFT, MutaStanga);
    auxKeyFunc(AUX_RIGHT, MutaDreapta);
    auxKeyFunc(AUX_UP, MutaSus);
    auxKeyFunc(AUX_DOWN, MutaJos);
    auxKeyFunc('s', MutaCoadaS);
    auxKeyFunc('w', MutaCoadaJ);
    auxReshapeFunc(myReshape);
    auxIdleFunc(display);
    auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, movelight);
    auxMainLoop(display);
    return 0;
}
