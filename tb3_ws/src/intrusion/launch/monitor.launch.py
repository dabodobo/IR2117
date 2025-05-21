from launch import LaunchDescription
from launch_ros.actions import Node
import launch.actions

def generate_launch_description():
	return LaunchDescription([
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='north',
		parameters=[
			{"obs_angle_min": -0.3927},
			{"obs_angle_max": 0.3927},
			{"obs_threshold": 2.0}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='west',
		parameters=[
			{"obs_angle_min": 1.3},
			{"obs_angle_max": 1.8},
			{"obs_threshold": 2.0}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='east',
		parameters=[
			{"obs_angle_min": -1.8},
			{"obs_angle_max": -1.3},
			{"obs_threshold": 2.0}]
	),
	
	Node(
		package='obstacles',
		executable='detector',
		namespace='northwest',
		parameters=[
			{"obs_angle_min": 0.5},
			{"obs_angle_max": 1.1},
			{"obs_threshold": 2.0}]
	),
	Node(
		package='obstacles',
		executable='detector',
		namespace='northeast',
		parameters=[
			{"obs_angle_min": -1.1},
			{"obs_angle_max": -0.5},
			{"obs_threshold": 2.0}]
	),
	Node(
		package='obstacles',
		executable='detector',
		namespace='south',
		parameters=[
			{"obs_angle_min": 2.7},
			{"obs_angle_max": -2.7},
			{"obs_threshold": 2.0}]
	),
	Node(
		package='obstacles',
		executable='detector',
		namespace='southwest',
		parameters=[
			{"obs_angle_min": 2.0},
			{"obs_angle_max": 2.5},
			{"obs_threshold": 2.0}]
	),
	Node(
		package='obstacles',
		executable='detector',
		namespace='southeast',
		parameters=[
			{"obs_angle_min": -2.5},
			{"obs_angle_max": -2.0},
			{"obs_threshold": 2.0}]
	),
	Node(
		package='intrusion',
		executable='monitor',
		
	),
	
	

])
