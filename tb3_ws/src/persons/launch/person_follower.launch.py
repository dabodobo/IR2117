from launch import LaunchDescription
from launch_ros.actions import Node
import launch.actions

def generate_launch_description():
	return LaunchDescription([
	
	Node(
		package='persons',
		executable='detector',
		namespace='front',
		parameters=[
			{"person_angle_min": -0.3927},
			{"person_angle_max": 0.3927},
			{"person_distance_min": 0.5},
			{"person_distance_max": 1.3}]
	),
	
	Node(
		package='persons',
		executable='detector',
		namespace='left',
		parameters=[
			{"person_angle_min": 0.3927},
			{"person_angle_max": 1.1781},
			{"person_distance_min": 0.5},
			{"person_distance_max": 1.3}]
	),
	
	Node(
		package='persons',
		executable='detector',
		namespace='right',
		parameters=[
			{"person_angle_min": -1.1781},
			{"person_angle_max": -0.3927},
			{"person_distance_min": 0.5},
			{"person_distance_max": 1.3}]
	),
	
	Node(
		package='persons',
		executable='follower'
	)
		
	

])
