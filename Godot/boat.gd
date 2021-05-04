extends KinematicBody

var player
var boat_label
var floating = false

# wave
var time = 0
var waveA = [-1,0,0.25,6]
var waveB = [-1,0.6,0.25,3]
var waveC = [-1,1.3,0.25,2]
var speed = 0.5

func _ready():
	player = get_node("../player")
	boat_label = get_node("../HUD/HUD/MarginContainer/CenterContainer/boat")
	
func receive_time(get_time):
	print(time," ",get_time)
	time = get_time

func _process(delta):
	time += delta
	float_boat()

func float_boat():
	measure_angle()
	#translation =  Vector3(translation.x,-0.5+offset.y,0)

func measure_angle():
	var back = compute_wave(Vector2(-2,0))
	var front = compute_wave(Vector2(2,0))
	var left = compute_wave(Vector2(0,-1))
	var right = compute_wave(Vector2(0,1))
	rotation_degrees.z = rad2deg(0.5*atan((front.y-back.y)/4.0))
	rotation_degrees.x = rad2deg(0.5*-atan((right.y-left.y)/2.0))

# compute the height of the wave
func compute_wave(position_offset):
	var pos_x = translation.x - 9.0 + 0.5 + position_offset.x
	var pos_y = position_offset.y
	var offset_old = gerstnerwave(waveA[0],waveA[1],waveA[2],waveA[3],pos_x,pos_y,time*speed)
	var offset = gerstnerwave(waveA[0],waveA[1],waveA[2],waveA[3],pos_x-offset_old.x,pos_y-offset_old.z,time*speed)
	offset_old = gerstnerwave(waveB[0],waveB[1],waveB[2],waveB[3],pos_x,pos_y,time*speed)
	offset += gerstnerwave(waveB[0],waveB[1],waveB[2],waveB[3],pos_x-offset_old.x,pos_y-offset_old.z,time*speed)
	offset_old = gerstnerwave(waveC[0],waveC[1],waveC[2],waveC[3],pos_x,pos_y,time*speed)
	offset += gerstnerwave(waveC[0],waveC[1],waveC[2],waveC[3],pos_x-offset_old.x,pos_y-offset_old.z,time*speed)
	return offset

# Gerstner Wave
func gerstnerwave(direction_x,direction_y,steepness,wavelength,position_x,position_y,_time):
	var k = 2.0*3.14159265359/wavelength
	var h = 5.0 + sqrt(position_x + 10.0)
	var c = sqrt(9.8*k*tanh(k*h))
	var d = Vector2(direction_x,direction_y).normalized()
	var f = k*Vector2(position_x,position_y).dot(d)-c*_time
	var a = steepness / k
	return Vector3(d.x * a * cos(f),a*sin(f),d.y * a * cos(f))

# boat switch
func _on_Area_body_entered(body):
	if body == player:
		player.in_boat = true
		boat_label.text = str("[E] ride boat")
		boat_label.visible = true
func _on_Area_body_exited(body):
	if body == player:
		player.in_boat = false
		boat_label.visible = false
func _on_climb_body_entered(body):
	if body == player:
		player.climb_boat = true
		boat_label.text = str("[E] climb the boat")
		boat_label.visible = true
func _on_climb_body_exited(body):
	if body == player:
		player.climb_boat = false
		boat_label.visible = false