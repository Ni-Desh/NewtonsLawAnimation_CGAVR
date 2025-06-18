#include <GL/glut.h>
#include <cmath>
#include <algorithm>

// Global variables
float ballX = -0.8f;   // Ball position (X-axis)
float ballY = 0.0f;    // Ball position (Y-axis, constant)
float velocity = 0.02f; // Ball velocity
float force = 0.001f;  // Applied force
float acceleration = 0.0f; // Acceleration due to force
int lawState = 1;      // Current law being demonstrated (1: First, 2: Second, 3: Third)

// Utility to draw text
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Utility to draw a rectangle (block or background)
void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Utility to draw an arrow (force visualization)
void drawArrow(float startX, float startY, float endX, float endY, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
    glEnd();

    float angle = atan2(endY - startY, endX - startX);
    float arrowSize = 0.05f;
    glBegin(GL_TRIANGLES);
    glVertex2f(endX, endY);
    glVertex2f(endX - arrowSize * cos(angle + M_PI / 6), endY - arrowSize * sin(angle + M_PI / 6));
    glVertex2f(endX - arrowSize * cos(angle - M_PI / 6), endY - arrowSize * sin(angle - M_PI / 6));
    glEnd();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw background
    drawRectangle(-1.0f, -0.5f, 2.0f, 0.5f, 0.9f, 0.9f, 0.9f); // Ground
    drawRectangle(-1.0f, 0.0f, 2.0f, 1.0f, 0.8f, 0.9f, 1.0f);  // Sky

    // Draw block (static object)
    drawRectangle(0.5f, -0.3f, 0.2f, 0.2f, 0.5f, 0.5f, 0.5f);

    // Draw ball
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(ballX + 0.05f * cos(theta), ballY + 0.05f * sin(theta));
    }
    glEnd();

    // Switch between laws
    if (lawState == 1) {
        drawText("Newton's First Law: Inertia", -0.9f, 0.9f);
    } else if (lawState == 2) {
        drawText("Newton's Second Law: F = ma", -0.9f, 0.9f);
        drawArrow(ballX, ballY, ballX + 0.2f * acceleration, ballY, 1.0f, 0.0f, 0.0f); // Acceleration
    } else if (lawState == 3) {
        drawText("Newton's Third Law: Action-Reaction", -0.9f, 0.9f);
        drawArrow(0.5f, -0.2f, 0.7f, -0.2f, 0.0f, 1.0f, 0.0f); // Action
        drawArrow(0.7f, -0.2f, 0.5f, -0.2f, 1.0f, 0.0f, 0.0f); // Reaction
    }

    glFlush();
}

// Timer function for animation
void timer(int value) {
    // Update ball position for First and Second Law
    if (lawState == 1 || lawState == 2) {
        ballX += velocity;
        if (lawState == 2) {
            acceleration += force; // Increase acceleration
            velocity += acceleration; // Increase velocity
        }

        // Collision and reset
        if (ballX > 1.0f) {
            ballX = -0.8f;
            velocity = 0.02f; // Reset velocity
            acceleration = 0.0f; // Reset acceleration
        }
    }

    glutPostRedisplay(); // Redraw the scene
    glutTimerFunc(16, timer, 0); // Call this function again after 16 ms (~60 FPS)
}

// Keyboard function for interaction
void keyboard(unsigned char key, int x, int y) {
    if (key == '1') {
        lawState = 1; // First Law
    } else if (key == '2') {
        lawState = 2; // Second Law
    } else if (key == '3') {
        lawState = 3; // Third Law
    } else if (key == '+') {
        force += 0.0005f; // Increase force
    } else if (key == '-') {
        force = std::max(0.0f, force - 0.0005f); // Decrease force
    } else if (key == 'p') {
        force = 0.0f; // Pause force
    } else if (key == 'r') {
        force = 0.001f; // Resume force
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Newton's Laws of Motion Animation");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set coordinate system

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0); // Start the animation
    glutKeyboardFunc(keyboard); // Handle keyboard input
    glutMainLoop();
    return 0;
}

