#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[]) 	
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");
  auto publisher = node->create_publisher<std_msgs::msg::String>("topic", 10);
  std_msgs::msg::String message;
  auto publish_count = 0;
  rclcpp::WallRate loop_rate(500ms);

  while (rclcpp::ok()) {
    message.data = "Hello, world! " + std::to_string(publish_count++);
    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}


/*
rclcpp:: library de ros
l1 include módulo sobre el tiempo (estándar de c++) para loop_rate
l3 para cada tipo de mensage un incclude
l9 iniciar
l10 node (un node por programa) variable de la clase rclcpp::Node, que hace de puntero a un
objeto de la clase Node.(make_shared) con el nombre "publisher"
l11 crea el publisher con el tipo de msgs que se enviaran por el topic llamado "topic" con un buffer de tamaño 10
l12 crea el mensaje de tipo String
l16 bool que indica si el ros va bien?
l22 Detener
l10 inicializa nodo --> objeto de la clase Node dentro de la biblioteca rclcpp
l11 creamos un objeto publisher dentro de node que publicara en un topic en formato string
l12 variable msg 
l16 True hasta que hagamos ctrl+c
l17 message.data contenido del mensaje
l18 publica
l19 Se ocupa de que no acabe el programa hasta hacer ctrl+c o se ocupa de cosas pendientes
l20 pausa de 500 ms

*/

