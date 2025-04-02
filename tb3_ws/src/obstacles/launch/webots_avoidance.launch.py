from launch import LaunchDescription
from launch_ros.actions import Node
import launch.actions

def generate_launch_description():
	return LaunchDescription([
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='front',
		parameters=[
			{"obs_angle_min": 2.7489},
			{"obs_angle_max": 3.5343},
			{"obs_threshold": 0.5}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='left',
		parameters=[
			{"obs_angle_min": 2.3256},
			{"obs_angle_max": 2.7489},
			{"obs_threshold": 0.5}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='right',
		parameters=[
			{"obs_angle_min": 3.5343},
			{"obs_angle_max": 4.3197},
			{"obs_threshold": 0.5}]
	),
	
	Node(
		package='obstacles',
		executable='avoidance'
	)
		
	

])
