#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define HEIGHT 480 // Height of Screen
#define WIDTH 640 // Width of Screen
#define BZC 50 // Number of interpolated points in between the control points

std::vector<float> positions; // Stores all the control points given by user

/* returns the bezier curve point for a given ratio (Uses recursion) */
std::vector<float> bezier_curve_point(std::vector<float> pos, float ratio)
{
  if (pos.size() % 3 != 0)
  {
    std::cout << "Error";
    return std::vector<float>(1,0);
  }

  if (pos.size() == 3) return pos;

  std::vector<float> new_pos;
  float temp;
  for (int i = 0; i < (pos.size() - 3); i++)
  {
    temp = (pos[i] * (1 - ratio)) + (pos[i+3] * ratio);
    new_pos.push_back(temp);
  }

  return bezier_curve_point(new_pos, ratio);
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

  float posfloat[3 * (BZC + 1)];
  for (int i = 0; i < (3 * (BZC + 1)); i++)
  {
    posfloat[i] = 0.0f;
  }

  GLuint vb;
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, 33 * sizeof(float), posfloat, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Gets the state of the left mouse button */
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    /* Is true whenever left button is clicked */
    if (state == GLFW_PRESS)
    {
      /* Get the postition of the mouse-click w.r.t the top-left corner */
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      std::cout << xpos << "~~" << ypos << "\n";

      /* Converting mouse coordinates to 3D normalized floats with current screen as z=0 plane */
      positions.push_back(2 * ((xpos / WIDTH) - 0.5));
      positions.push_back(2 * (0.5 - (ypos / HEIGHT)));
      positions.push_back(0.0f);

      /* Need this as a click lasts few milliseconds */
      usleep(200000);

      /* Prints all the control points given by user */
      for (int i = 0; i < positions.size(); i++)
      {
        std::cout << positions[i] << ", ";
      }
      std::cout << "\n";

      /* Stores the newly processed Bezier Curve interplotaed points into the Vertex Buffer */
      float n = BZC;
      for (float i = 0; i <= n; i++)
      {
        std::vector<float> pos = bezier_curve_point(positions, (i/n));
        for (int j = 0; j < pos.size(); j++)
        {
          posfloat[int(3 * i + j)] = pos[j];
        }
      }

      /* If you wish to print the Vertx Buffer, uncomment this */
      // for (int i = 0; i < (3 * (BZC + 1)); i++)
      // {
      //   std::cout << posfloat[i] << ", ";
      // }
      // std::cout << "\n";

    }

    /* Updates buffer */
    glBufferData(GL_ARRAY_BUFFER, 3 * (BZC + 1) * sizeof(float), posfloat, GL_DYNAMIC_DRAW);

    /* Joins all the interpolated points, thus creating the approximated Bezier Curve */
    for (int i = 0; i < BZC; i++)
    {
      glDrawArrays(GL_LINE_STRIP, i, 2);
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  /* If you wish to print the final Vertx Buffer, uncomment this */
  // for (int i = 0; i < (3 * (BZC + 1)); i++)
  // {
  //   std::cout << posfloat[i] << ", ";
  // }
  // std::cout << "\n";

  glfwTerminate();
  return 0;
}
