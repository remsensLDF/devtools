#include <iostream>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include "ResourceManager/ResourceHandle.h"
#include <cxxopts.hpp>
#include "array_core/array_core.h"
#include "common_utils/common_constants.h"
#include "common_utils/common_utils.h"

using std::vector;

int main(int argc, char* argv[]) {

  cxxopts::Options options("davis", "data visualization utility");
  options.add_options()
         ("h,help", "davis commands")
         ("l,linechart", "linechart values", cxxopts::value<std::string>())
         ("m,heatmap", "heatmap values", cxxopts::value<std::string>())
         ("s,surface", "surface values", cxxopts::value<std::string>())
         ("f,datapath", "path to data", cxxopts::value<std::string>())
         ("t,charttype", "chart type", cxxopts::value<std::string>())
         ;
  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }
  // here we have to copy plotly js to current dir if it doesn't exist
  ResourceHandle resource_handle(dvs::kPlotlyJsResourcePath);
  if (!dvs::isPlotlyScriptExists()) {
    dvs::mayBeCreateJsWorkingFolder();
    dvs::saveStringToFile(dvs::kPlotlyJsWorkPath, resource_handle.c_str());
  }


  vector<vector<double>> values = {{30.3, 40, 98, 76}, {99, 45, 20, 1}, {5, 56, 93, 25}, {45, 23, 90, 2}};

  dv::show(values, "testHeat1");
  auto config = dv::Config();

  if (result.count("linechart")) {
    auto data = result["linechart"].as<std::string>();
    config.common.typeVisual = dv::VISUALTYPE_CHART;
    dvs::showLineChartInBrowser(data, "comand_line_linechart", config);
    return EXIT_SUCCESS;
  } else if (result.count("heatmap")) {
    auto data = result["heatmap"].as<std::string>();
    config.common.typeVisual = dv::VISUALTYPE_HEATMAP;
    dvs::showHeatMapInBrowser(data, "comand_line_heatmap", config);
    return EXIT_SUCCESS;
  } else if (result.count("surface")) {
    config.common.typeVisual = dv::VISUALTYPE_SURFACE;
    auto data = result["surface"].as<std::string>();
    dvs::showSurfaceInBrowser(data, "comand_line_surface", config);
    return EXIT_SUCCESS;
  } else if (result.count("datapath")) {
    auto data_path = result["datapath"].as<std::string>();
    std::string str_data;
    if (dvs::getDataFromFile(data_path, str_data)) {
      if (result.count("charttype")) {
        auto chart_type = result["charttype"].as<std::string>();
        if (chart_type == "l" || chart_type == "linechart") {
          config.common.typeVisual = dv::VISUALTYPE_CHART;
          dvs::showLineChartInBrowser(str_data, "file_data", config);
        } else if (chart_type == "s" || chart_type == "surface") {
          config.common.typeVisual = dv::VISUALTYPE_SURFACE;
          dvs::showSurfaceInBrowser(str_data, "surface", config);
        } else if (chart_type == "m" || chart_type == "heatmap") {

        }
      } else {
        config.common.typeVisual = dv::VISUALTYPE_HEATMAP;
        dvs::showHeatMapInBrowser(str_data, "file_data", config);
      }
    }
  }

  return EXIT_SUCCESS;
}


