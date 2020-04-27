#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define HEIGHT 480
#define WIDTH 640

std::vector<float> positions;

/* returns the bezier curve point for a given ratio (Uses recursion) */
std::vector<float> bezier_curve_point(std::vector<float> pos, float ratio)
{
  if (pos.size() % 3 != 0)
  {
    std::cout << "Error";
    return std::vector<float>(1,0);
  }

  std::vector<float> new_pos;
  float temp;
  for (int i = 0; i < (pos.size() - 3); i++)
  {
    temp = (pos[i] * (1 - ratio)) + (pos[i+3] * ratio);
    new_pos.push_back(temp);
  }

  if (new_pos.size() == 3) return new_pos;

  return bezier_curve_point(new_pos, ratio);
}

/* It is called whenever mouse is clicked */
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

  /* Initialize the glfw3 library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Initialize glew library */
  if (glewInit() != GLEW_OK)
    return -1;

  std::cout << glGetString(GL_VERSION) << std::endl;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Get the postition of the mouse-click w.r.t the top-left corner */
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      std::cout << xpos << "~~" << ypos << "\n";
      /* Converting mouse coordinates to 3D normalized floats with current screen as z=0 plane */
      positions.push_back(2 * ((xpos / WIDTH) - 0.5));
      positions.push_back(2 * (0.5 - (ypos / HEIGHT)));
      positions.push_back(0.0f);
      /* Need this as a click lasts few milliseconds */
      usleep(200000);
    }
    /* Poll for and process events */
    glfwPollEvents();
  }

  /* Prints the control points given by user */
  for (int i = 0; i < positions.size(); i++)
  {
    std::cout << positions[i] << ", ";
  }
  std::cout << "\n";

  /* Prints the returned bezier curve points. */
  float n = 10;
  for (float i = 0; i <= n; i++)
  {
    std::vector<float> pos = bezier_curve_point(positions, (i/n));
    for (int i = 0; i < pos.size(); i++)
    {
      std::cout << pos[i] << ", ";
    }
    std::cout << "\n";
  }

  glfwTerminate();
  return 0;
}
