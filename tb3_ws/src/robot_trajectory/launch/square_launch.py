from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
	return LaunchDescription([
		Node(
			package= 'robot_trajectory',
			executable = 'turtle_square',
			remappings=[
				('/cmd_vel', 'turtle1/cmd_vel'),
			],
			parameters=[
				{"angular_speed":0.8},
				{"distance_between_loops":1.5},
				{"number_of_loops":2.4}
			]
		)
		
	])	
