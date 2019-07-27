#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr int PORT = 8080;
static std::string IP = "34.83.142.125";

// flags
static bool DEBUG = false;

// global network config
static int sockfd;
static struct sockaddr_in servaddr;

// parse CLI options
// true == error
static bool parse_opts(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (std::string(argv[i]).compare("--debug") == 0) {
      DEBUG = true;
    } else if (std::string(argv[i]).compare("--ip") == 0) {
      if (i + 2 < argc) {
        return true;
      }
      IP = std::string(argv[i + 1]);
    }
  }
  return false;
}

// initialize network configuration // true == error
static bool init_network() {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Socket creation failed!" << std::endl;
    return true;
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(IP.c_str());
  return false;
}

// send greeting to server
static std::array<int, 4> greet() {
  std::cout << "Sending greeting..." << std::endl;
  const std::string hello = "hello";
  sendto(sockfd, hello.c_str(), hello.size(), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Greeted server!" << std::endl;

  std::cout << "Waiting for response..." << std::endl;
  std::vector<char> buffer(1024);
  int n = recvfrom(sockfd, &buffer[0], buffer.size(), MSG_WAITALL,
                   nullptr, nullptr);
  buffer[n] = 0;

  std::string response(buffer.begin(), buffer.begin() + n);
  std::cout << "Server responded: " << std::move(response) << std::endl;

  return {{{}, {}, {}, {}}};
}

static void block_hit(int i) {
  std::cout << "Sending hit " << i << "..." << std::endl;
  const auto str = std::to_string(i);
  sendto(sockfd, str.c_str(), str.size(), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Sent hit!" << std::endl;
}

static void mousecb(GLFWwindow* window, int button, int action, int mode) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    block_hit(1);
  }
}

int main(int argc, char* argv[]) {
  if (parse_opts(argc, argv) || init_network()) {
    return 1;
  }

  auto coordinates = greet();

  if (!glfwInit()) {
    std::cerr << "glfwInit() failed" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
  if (!window) {
    std::cerr << "window creation failed" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwSetMouseButtonCallback(window, mousecb);
  glfwMakeContextCurrent(window);

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  while (!glfwWindowShouldClose(window)) {
    GLint width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(60, (double) width / height, 0.1, 100);

    glMatrixMode(GL_MODELVIEW_MATRIX);
    glTranslatef(0, 0, -5);

    // draw
    constexpr GLfloat vtx[] = {
      -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    constexpr GLfloat colors[] =
    {
        1, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
    };

    static GLfloat alpha = 0;

    glRotatef(alpha, 0, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vtx);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 0.1;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  close(sockfd);
  return 0;
}
