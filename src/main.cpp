#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

const int cycles = 25000;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

void restart(uWS::WebSocket<uWS::SERVER> ws) {
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
}

int main(int argc, char *argv[]) {
  uWS::Hub h;
  int iteration = 0;
  int low_speed_counter = 0;
  double init_Kp, init_Ki,init_Kd;
  if (argc == 3) {
    init_Kp = atof(argv[1]);
    init_Ki = atof(argv[2]);
    init_Kd = atof(argv[3]);
  } else {
    init_Kp = 2.473301013746727;
    init_Ki = 0.0;
    init_Kd = 12.317216854917202;
  }
  PID pid;
  pid.Init(init_Kp, init_Ki, init_Kd);

  h.onMessage([&pid, &iteration, &low_speed_counter](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    iteration++;
    if (iteration == 1) {
      restart(ws); //restart in first cycle
    } else if (iteration > cycles) { //if cycles are completed restart and close socket
      std::cout << pid.AccumError() << std::endl;;
      restart(ws);
      ws.close();
    } else {
      // "42" at the start of the message means there's a websocket message event.
      // The 4 signifies a websocket message
      // The 2 signifies a websocket event
      if (length && length > 2 && data[0] == '4' && data[1] == '2') {
        auto s = hasData(string(data).substr(0, length));

        if (s != "") {
          auto j = json::parse(s);

          string event = j[0].get<string>();

          if (event == "telemetry") {
            // j[1] is the data JSON object
            double cte = std::stod(j[1]["cte"].get<string>());
            double speed = std::stod(j[1]["speed"].get<string>());
            double angle = std::stod(j[1]["steering_angle"].get<string>());
            double steer_value;
            
            if (speed < 5) { //checking if our vehicle stuck in this cycle
              low_speed_counter++;
            } else {
              low_speed_counter = 0;
            }
            if (low_speed_counter > 400) { //if vehicle stuck, restart simulator and close the programm
              std::cout << pid.AccumError() + (cycles - iteration) * cte * cte  << std::endl; //adding to accum error the left cycles with current cte
              restart(ws);
              ws.close();
              return;
            }
            pid.UpdateError(cte);
            
            steer_value = pid.TotalError();

            json msgJson;
            msgJson["steering_angle"] = steer_value;
            msgJson["throttle"] = (speed > 20) ? 0.2 : 0.3; //for keeping speed around 20 MPH
            auto msg = "42[\"steer\"," + msgJson.dump() + "]";
            //std::cout << msg << std::endl;
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }  // end "telemetry" if
        } else {
          // Manual driving
          string msg = "42[\"manual\",{}]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      }  // end websocket message if
    }
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    //std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
//    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
