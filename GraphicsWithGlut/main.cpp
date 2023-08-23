#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const void* FONT_NAME = GLUT_BITMAP_HELVETICA_18;







enum BodyPart {
    NONE,
    HEAD,
    HAND,
    LEG,
    BODY,
    EYE,
    NOSE,
    LIPS,
};

BodyPart selectedPart = NONE;
bool isDragging = false;
float dragOffsetX = 0.0;
float dragOffsetY = 0.0;

float headRadius = 0.15;
float leftArmX = -0.3;
float leftArmY = 0.1;
float rightArmX = 0.3;
float rightArmY = 0.1;
float leftLegX =  0.4;
float RightLegX = 0.4;
float leftLegY = -0.4;
float rightLegY = -0.4;

void DrawCircle(float cx, float cy, float radius, int numSegments)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; ++i) {
        float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void DrawEyes()
{
    glColor3f(1.0, 1.0, 1.0); // white color

    // Left eye
    DrawCircle(-0.05, 0.6, 0.03, 100);

    // Right eye
    DrawCircle(0.05, 0.6, 0.03, 100);
}

void DrawNose()
{
    glColor3f(1.0, 0.5, 0.0); // Orange color

    // Draw nose (triangle)
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.55);
    glVertex2f(-0.02, 0.5);
    glVertex2f(0.02, 0.5);
    glEnd();
}

void DrawLips()
{
    glColor3f(1.0, 0.0, 0.0); // Red color

    // Draw lips (curve)
    glBegin(GL_LINE_STRIP);
    for (float angle = 0; angle <= M_PI; angle += 0.01) {
        float x = 0.05 * cos(angle);
        float y = 0.48 + 0.02 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawDoll()
{
    glColor3f(0.0, 0.0, 1.0); // Blue color

    // Draw head (circle)
    DrawCircle(0.0, 0.5, headRadius, 100);

    // Draw body (rectangle)
    glBegin(GL_POLYGON);

    glVertex2f(-0.1, 0.5);
    glVertex2f(0.1, 0.5);
    glVertex2f(0.1, 0.0);
    glVertex2f(-0.1, 0.0);
    glEnd();

    // Draw arms (lines)
    glLineWidth(6.0); // Set thicker line width for arms
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.15, 0.4);
    glVertex2f(leftArmX, leftArmY);
    glVertex2f(0.15, 0.4);
    glVertex2f(rightArmX, rightArmY);
    glEnd();

    // Draw legs (lines)
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-0.05, 0.0);
    glVertex2f(-0.05, leftLegY);
    glVertex2f(0.05, 0.0);
    glVertex2f(0.05, rightLegY);
    glEnd();

    // Draw facial features
    DrawEyes();
    DrawNose();
    DrawLips();
}

void DrawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter((void*)FONT_NAME, *c);
    }
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    DrawDoll();

    // Show a message for each touched body part
    if (selectedPart != NONE) {
        const char* partName = "Unknown Part";
        switch (selectedPart) {
            case HEAD:
                partName = "Head";
                break;
            case HAND:
                partName = "Hand";
                break;
            case LEG:
                partName = "Leg";
                break;
            case BODY:
                partName = "Body";
                break;
            case EYE:
                partName = "Eye";
                break;
            case NOSE:
                partName = "Nose";
                break;
            case LIPS:
                partName = "Lips";
                break;
            default:
                break;
        }
        glColor3f(0.0, 0.0, 0.0);
        DrawText(-0.9, 0.9, partName);
    }

    glFlush();
}

void Initialize()
{
    glClearColor(1, 1, 1, 1);
}

void MouseFunc(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mouseX = (x / static_cast<float>(WINDOW_WIDTH)) * 2.0 - 1.0;
        float mouseY = ((WINDOW_HEIGHT - y) / static_cast<float>(WINDOW_HEIGHT)) * 2.0 - 1.0;

       // if (mouseX >= -0.1 && mouseX <= 0.1 && mouseY >= 0.5 - headRadius && mouseY <= 0.5 + headRadius)
          //  selectedPart = HEAD;
        if (mouseX >= -0.3 && mouseX <= -0.15 && mouseY >= 0.1 && mouseY <= 0.4)
            selectedPart = HAND;
        else if (mouseX >= 0.15 && mouseX <= 0.3 && mouseY >= 0.1 && mouseY <= 0.4)
            selectedPart = HAND;
        else if (mouseX >= -0.05 && mouseX <= 0.05 && mouseY >= leftLegY && mouseY <= 0.0)
            selectedPart = LEG;
        else if (mouseX >= -0.15 && mouseX <= -0.05 && mouseY >= leftLegY && mouseY <= 0.0)
            selectedPart = LEG;
        else if (mouseX >= -0.07 && mouseX <= 0.07 && mouseY >= 0.6 && mouseY <= 0.64)
            selectedPart = EYE;
        else if (mouseX >= -0.02 && mouseX <= 0.02 && mouseY >= 0.5 && mouseY <= 0.55)
            selectedPart = NOSE;
        else if (mouseX >= -0.06 && mouseX <= 0.06 && mouseY >= 0.46 && mouseY <= 0.48)
            selectedPart = LIPS;
        else if (mouseX >= -0.1 && mouseX <= 0.1 && mouseY >= 0.5 - headRadius && mouseY <= 0.5 + headRadius)
            selectedPart = HEAD;
        else if (mouseX >= -0.1 && mouseX <= 0.1 && mouseY >= 0.0 && mouseY <= 0.5)
            selectedPart = BODY;


        else
            selectedPart = NONE;

        if (selectedPart != NONE) {
            isDragging = true;
            dragOffsetX = mouseX;
            dragOffsetY = mouseY;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDragging = false;
    }

    glutPostRedisplay();
}

void MouseMotionFunc(int x, int y)
{
    if (isDragging) {
        float mouseX = (x / static_cast<float>(WINDOW_WIDTH)) * 2.0 - 1.0;
        float mouseY = ((WINDOW_HEIGHT - y) / static_cast<float>(WINDOW_HEIGHT)) * 2.0 - 1.0;

        switch (selectedPart) {
            case HEAD:
                headRadius += (mouseY - dragOffsetY) * 0.1;
                if (headRadius < 0.05) headRadius = 0.05;
                dragOffsetY = mouseY;
                break;
            case HAND:
                leftArmX += mouseX - dragOffsetX;
                leftArmY += mouseY - dragOffsetY;
                rightArmX += mouseX - dragOffsetX;
                rightArmY += mouseY - dragOffsetY;
                dragOffsetX = mouseX;
                dragOffsetY = mouseY;
                break;
            case LEG:
                leftLegX += mouseX - dragOffsetX;
                leftLegY += mouseY - dragOffsetY;
                RightLegX += mouseX - dragOffsetX;
                rightLegY += mouseY - dragOffsetY;
                dragOffsetX = mouseX;
                dragOffsetY = mouseY;
                break;
            default:
                break;
        }

        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 250);
    glutCreateWindow("Interactive Doll with GLUT");
    Initialize();
    glutDisplayFunc(Draw);
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MouseMotionFunc);
    glutMainLoop();
    return 0;
}
