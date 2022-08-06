#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lib/ImGui/imgui_impl_glfw.h"
#include "lib/ImGui/imgui_impl_opengl3.h"
#include "lib/ImGui/imgui.h"

#include "lib/ImGui/implot.h"
#include "lib/ImGui/implot_internal.h"

#define CSVData std::vector<std::string>*

static bool open = false;
static int dwidth = 900;
static int dheight = 600;

static char* csvPath;
static CSVData csvData;
static size_t lineCount = 0;
static int currentItem = 1;

void theme(){
#define HIGHLIGHT_COLOR(v) ImVec4(0.502f, 0.075f, 0.255f, v)
    auto &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBgActive] = HIGHLIGHT_COLOR(1.00f);

    style.WindowPadding = ImVec2(10, 5);
    style.WindowRounding = 2.5f;
    style.GrabMinSize = 30.0f;

    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.485f, 0.235f, 0.163f);
    style.WindowBorderSize = 2.5f;
}

void handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        exit(0);
    }
}

std::vector<std::string> split(std::string content, char delimiter) {
	int selector = 0;
	std::string part = content;
	std::vector<std::string> sp;

	int count = 0;
	for(char i : part) {
		if (i == delimiter) {
			count++;
		}
	}

	for(int i = 0; i <= count; i++){
		int selectNext = part.find(delimiter, selector);
		std::string prt = part.substr(selector, selectNext - selector);
		sp.push_back(prt);
		selector = ++selectNext;
	}

	return sp;
}

int timeToSeconds(const std::string& time) {
	auto splitted = split(time, ':');
	int hoursInSeconds = ((std::stoi(splitted[0]) * 60) * 60);
	int minutesInSeconds = std::stoi(splitted[1]) * 60;
	int seconds = std::stoi(split(splitted[2], ',')[0]);

	return hoursInSeconds + minutesInSeconds + seconds;
}

std::string secondsToTime(const int seconds) {
	int minutes = (seconds - (seconds % 60)) / 60;
	int hours = (minutes - (minutes % 60)) / 60;

	minutes = minutes - (hours * 60);

	std::string result;
	if(hours > 9) {
		result.append(std::to_string(hours));
	} else {
		result.append("0").append(std::to_string(hours));
	}
	result.append(":");

	if(minutes > 9) {
		result.append(std::to_string(minutes));
	} else {
		result.append("0").append(std::to_string(minutes));
	}
	result.append(":");

	if(seconds > 9) {
		result.append(std::to_string(seconds));
	} else {
		result.append("0").append(std::to_string(seconds));
	}

	return result;
}

CSVData loadCSVData(const char* path) {
    std::ifstream file(path);

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line)) {
        lines.push_back(line);
    }
	lineCount = lines.size();

	CSVData data = new std::vector<std::string>[lines.size()]; // data.content[0] = die Header
    data[0] = split(lines.at(0), ',');

    for(int i = 1; i < lines.size(); i++) {
        auto l = split(lines.at(i), ',');
        for(const auto& j : l) {
            data[i].push_back(j);
        }
    }

    return data;
}

void drawListBox(const std::vector<std::string>& elements) {
    if(ImGui::BeginListBox("Listbox")) {
        for(int i = 1; i < elements.size(); i++) {
            const bool is_selected = (currentItem == i);
            if(ImGui::Selectable(elements.at(i).c_str(), is_selected)) {
	            currentItem = i;
            }

            if(is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndListBox();
    }
}

void drawPlot() {
	int timeData[lineCount-1];
	for(int i = 1; i < lineCount; i++) {
		timeData[i-1] = timeToSeconds(csvData[i].at(0));
	}


	int dataData[lineCount-1];
	for(int i = 1; i < lineCount; i++) {
		auto data = std::stoi(csvData[i].at(currentItem)); // i = zeile, currentItem = Spalte
		dataData[i-1] = data;
	}


	if(ImPlot::BeginPlot("CSV Data")) {
		ImPlot::SetupAxes("Time", "Data");
		ImPlot::PlotLine(csvData[0].at(currentItem).c_str(), timeData, dataData, lineCount-1);
		ImPlot::EndPlot();
	}
}

void drawUI(){
    ImGui::SetNextWindowSize(ImVec2(dwidth, dheight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("CSV Graph", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	char* buffer;

    ImGui::Text("Path-To-CSV");
    ImGui::SameLine();
    ImGui::InputText(" - ", buffer, sizeof(buffer)*10);
    ImGui::SameLine();

    if(ImGui::Button("Load")) {
	    csvData = loadCSVData(buffer);
    }

	if(csvData != nullptr) {
		drawListBox(csvData[0]);
		drawPlot();
	}

    ImGui::End();
}

GLFWwindow* setup() {
    if(!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* win = glfwCreateWindow(dwidth, dheight, "CSV Graphs", NULL, NULL);

    glfwSetKeyCallback(win, handleKeyboard);
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    bool init = glewInit() != GLEW_OK;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    const char* v = "#version 130";
    ImGui_ImplOpenGL3_Init(v);

    return win;
}

int main() {
    GLFWwindow* win = setup();

    theme();

    while(!glfwWindowShouldClose(win)){
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();

        glfwGetFramebufferSize(win, &dwidth, &dheight);
        glViewport(0, 0, dwidth, dheight);

        ImGui::Render();
        glClearColor(0.31f, 0.31f, 0.31f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);
    }

	ImPlot::DestroyContext();
	ImGui::DestroyContext();


    return 0;
}