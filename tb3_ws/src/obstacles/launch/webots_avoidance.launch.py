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
			{"obs_angle_max": 1.},
			{"obs_threshold": 0.5}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='right',
		parameters=[
			{"obs_angle_min": -1.1781},
			{"obs_angle_max": -0.3927},
			{"obs_threshold": 0.5}]
	),
	
	Node(
		package='obstacles',
		executable='avoidance'
	)
		
	

])
