from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
	return LaunchDescription([

		Node(
			package= 'robot_trajectory',
			executable = 'polygon',
			parameters=[
				{"segment_size":1.25},
				{"number_segments":6},
				{"linear_speed":0.8},
				{"anglular_speed":0.5}
			]
		)
		
	])	
