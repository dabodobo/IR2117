cmake_minimum_required(VERSION 3.8)
project(suma)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
  
  find_package(rclcpp REQUIRED)
  find_package(std_msgs REQUIRED)
  
  add_executable(sum src/sumador.cpp)
  add_executable(pub src/publisher.cpp)
  
  ament_target_dependencies(sum rclcpp std_msgs)
  ament_target_dependencies(pub rclcpp std_msgs)
  
  install(TARGETS
  	sum
  	pub
  	DESTINATION lib/${PROJECT_NAME}
  
  )
  
endif()

ament_package()
