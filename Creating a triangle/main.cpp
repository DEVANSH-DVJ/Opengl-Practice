#include <GLFW/glfw3.h>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void mouse_button_callback(GLFWwindow* window, double xpos, double ypos)
{
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  // std::cout << state << "~~\n";
  if (state == GLFW_PRESS)
  {
    std::cout << xpos << "\n" << ypos;
  }
}

int main()
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.0f, 0.5f);
    glVertex2f( 0.5f, -0.5f);

    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Get the postition of the mouse-click w.r.t the top-left corner */
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      std::cout << xpos << "~~" << ypos << "\n";
      usleep(200000);
    }
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
