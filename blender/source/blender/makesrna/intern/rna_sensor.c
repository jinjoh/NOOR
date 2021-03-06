/**
 * $Id: rna_sensor.c 23463 2009-09-24 21:22:24Z ben2610 $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor(s): Blender Foundation (2008).
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <stdlib.h>

#include "RNA_define.h"
#include "RNA_types.h"

#include "rna_internal.h"

#include "DNA_sensor_types.h"

#ifdef RNA_RUNTIME

static StructRNA* rna_Sensor_refine(struct PointerRNA *ptr)
{
	bSensor *sensor= (bSensor*)ptr->data;

	switch(sensor->type) {
		case SENS_ALWAYS:
			return &RNA_AlwaysSensor;
		case SENS_TOUCH:
			return &RNA_TouchSensor;
		case SENS_NEAR:
			return &RNA_NearSensor;
		case SENS_KEYBOARD:
			return &RNA_KeyboardSensor;
		case SENS_PROPERTY:
			return &RNA_PropertySensor;
		case SENS_ARMATURE:
			return &RNA_ArmatureSensor;
		case SENS_MOUSE:
			return &RNA_MouseSensor;
		case SENS_COLLISION:
			return &RNA_CollisionSensor;
		case SENS_RADAR:
			return &RNA_RadarSensor;
		case SENS_RANDOM:
			return &RNA_RandomSensor;
		case SENS_RAY:
			return &RNA_RaySensor;
		case SENS_MESSAGE:
			return &RNA_MessageSensor;
		case SENS_JOYSTICK:
			return &RNA_JoystickSensor;
		case SENS_ACTUATOR:
			return &RNA_ActuatorSensor;
		case SENS_DELAY:
			return &RNA_DelaySensor;
		default:
			return &RNA_Sensor;
	}
}

#else

static void rna_def_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem sensor_type_items[] ={
		{SENS_ALWAYS, "ALWAYS", 0, "Always", ""},
		{SENS_TOUCH, "TOUCH", 0, "Touch", ""},
		{SENS_NEAR, "NEAR", 0, "Near", ""},
		{SENS_KEYBOARD, "KEYBOARD", 0, "Keyboard", ""},
		{SENS_PROPERTY, "PROPERTY", 0, "Property", ""},
		{SENS_MOUSE, "MOUSE", 0, "Mouse", ""},
		{SENS_COLLISION, "COLLISION", 0, "Collision", ""},
		{SENS_RADAR, "RADAR", 0, "Radar", ""},
		{SENS_RANDOM, "RANDOM", 0, "Random", ""},
		{SENS_RAY, "RAY", 0, "Ray", ""},
		{SENS_MESSAGE, "MESSAGE", 0, "Message", ""},
		{SENS_JOYSTICK, "JOYSTICK", 0, "joystick", ""},
		{SENS_ACTUATOR, "ACTUATOR", 0, "Actuator", ""},
		{SENS_DELAY, "DELAY", 0, "Delay", ""},
		{SENS_ARMATURE, "ARMATURE", 0, "Armature", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "Sensor", NULL);
	RNA_def_struct_ui_text(srna, "Sensor", "Game engine logic brick to detect events.");
	RNA_def_struct_sdna(srna, "bSensor");
	RNA_def_struct_refine_func(srna, "rna_Sensor_refine");

	prop= RNA_def_property(srna, "name", PROP_STRING, PROP_NONE);
	RNA_def_property_ui_text(prop, "Name", "Sensor name.");
	RNA_def_struct_name_property(srna, prop);

	/* type is not editable, would need to do proper data free/alloc */
	prop= RNA_def_property(srna, "type", PROP_ENUM, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE);
	RNA_def_property_enum_items(prop, sensor_type_items);
	RNA_def_property_ui_text(prop, "Type", "");

	prop= RNA_def_property(srna, "invert", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_ui_text(prop, "Invert Output", "Invert the level(output) of this sensor.");

	prop= RNA_def_property(srna, "level", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_ui_text(prop, "Level", "Level detector, trigger controllers of new states(only applicable upon logic state transition).");

	prop= RNA_def_property(srna, "pulse_true_level", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "pulse", SENS_PULSE_REPEAT);
	RNA_def_property_ui_text(prop, "Pulse True Level", "Activate TRUE level triggering (pulse mode).");

	prop= RNA_def_property(srna, "pulse_false_level", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "pulse", SENS_NEG_PULSE_MODE);
	RNA_def_property_ui_text(prop, "Pulse False Level", "Activate FALSE level triggering (pulse mode).");
	
	prop= RNA_def_property(srna, "frequence", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "freq");
	RNA_def_property_ui_text(prop, "Frequency", "Delay between repeated pulses(in logic tics, 0=no delay).");
	RNA_def_property_range(prop, 0, 10000);
}

static void rna_def_always_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	srna= RNA_def_struct(brna, "AlwaysSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Always Sensor", "Sensor to generate continuous pulses.");
}

static void rna_def_near_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "NearSensor", "Sensor");
	RNA_def_struct_ui_text(srna , "Near Sensor", "Sensor to detect nearby objects.");
	RNA_def_struct_sdna_from(srna, "bNearSensor", "data");

	prop= RNA_def_property(srna, "property", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "name");
	RNA_def_property_ui_text(prop, "Property", "Only look for objects with this property.");

	prop= RNA_def_property(srna, "distance", PROP_FLOAT, PROP_NONE);
	RNA_def_property_float_sdna(prop, NULL, "dist");
	RNA_def_property_ui_text(prop, "Distance", "Trigger distance.");
	RNA_def_property_range(prop, 0.0f, 10000.0f);

	prop= RNA_def_property(srna, "reset_distance", PROP_FLOAT, PROP_NONE);
	RNA_def_property_float_sdna(prop, NULL, "resetdist");
	RNA_def_property_ui_text(prop, "Reset Distance", "");
	RNA_def_property_range(prop, 0.0f, 10000.0f);
}

static void rna_def_mouse_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	static EnumPropertyItem mouse_event_items[] ={
		{BL_SENS_MOUSE_LEFT_BUTTON, "LEFTCLICK", 0, "Left Button", ""},
		{BL_SENS_MOUSE_MIDDLE_BUTTON, "MIDDLECLICK", 0, "Middle Button", ""},
		{BL_SENS_MOUSE_RIGHT_BUTTON, "RIGHTCLICK", 0, "Right Button", ""},
		{BL_SENS_MOUSE_WHEEL_UP, "WHEELUP", 0, "Wheel Up", ""},
		{BL_SENS_MOUSE_WHEEL_DOWN, "WHEELDOWN", 0, "Wheel Down", ""},
		{BL_SENS_MOUSE_MOVEMENT, "MOVEMENT", 0, "Movement", ""},
		{BL_SENS_MOUSE_MOUSEOVER, "MOUSEOVER", 0, "Mouse Over", ""},
		{BL_SENS_MOUSE_MOUSEOVER_ANY, "MOUSEOVERANY", 0, "Mouse Over Any", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "MouseSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Mouse Sensor", "Sensor to detect mouse events.");
	RNA_def_struct_sdna_from(srna, "bMouseSensor", "data");

	prop= RNA_def_property(srna, "mouse_event", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "type");
	RNA_def_property_enum_items(prop, mouse_event_items);
	RNA_def_property_ui_text(prop, "Mouse Event", "Specify the type of event this mouse sensor should trigger on.");
}

static void rna_def_touch_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "TouchSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Touch Sensor", "Sensor to detect objects colliding with the current object.");
	RNA_def_struct_sdna_from(srna, "bTouchSensor", "data");

	prop= RNA_def_property(srna, "material", PROP_POINTER, PROP_NONE);
	RNA_def_property_pointer_sdna(prop, NULL, "ma");
	RNA_def_property_flag(prop, PROP_EDITABLE);
	RNA_def_property_ui_text(prop, "Material", "Only look for floors with this material.");
}

static void rna_def_keyboard_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "KeyboardSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Keyboard Sensor", "Sensor to detect keyboard events.");
	RNA_def_struct_sdna_from(srna, "bKeyboardSensor", "data");

	prop= RNA_def_property(srna, "key", PROP_INT, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE); /* need better range or enum check */
	RNA_def_property_ui_text(prop, "Key", "Input key code.");
	RNA_def_property_range(prop, 0, 255);

	prop= RNA_def_property(srna, "modifier_key", PROP_INT, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE); /* need better range or enum check */
	RNA_def_property_int_sdna(prop, NULL, "qual");
	RNA_def_property_ui_text(prop, "Modifier Key", "Modifier key code.");
	RNA_def_property_range(prop, 0, 255);

	prop= RNA_def_property(srna, "second_modifier_key", PROP_INT, PROP_NONE);
	RNA_def_property_clear_flag(prop, PROP_EDITABLE); /* need better range or enum check */
	RNA_def_property_int_sdna(prop, NULL, "qual2");
	RNA_def_property_ui_text(prop, "Second Modifier Key", "Modifier key code.");
	RNA_def_property_range(prop, 0, 255);

	prop= RNA_def_property(srna, "target", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "targetName");
	RNA_def_property_ui_text(prop, "Target", "Property that indicates whether to log keystrokes as a string.");

	prop= RNA_def_property(srna, "log", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "toggleName");
	RNA_def_property_ui_text(prop, "Log", "Property that receive the keystrokes in case a string is logged.");

	prop= RNA_def_property(srna, "all_keys", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "type", 1);
	RNA_def_property_ui_text(prop, "All Keys", "Trigger this sensor on any keystroke.");
}

static void rna_def_property_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem prop_type_items[] ={
		{SENS_PROP_EQUAL, "PROPEQUAL", 0, "Equal", ""},
		{SENS_PROP_NEQUAL, "PROPNEQUAL", 0, "Not Equal", ""},
		{SENS_PROP_INTERVAL, "PROPINTERVAL", 0, "Interval", ""},
		{SENS_PROP_CHANGED, "PROPCHANGED", 0, "Changed", ""},
		/* {SENS_PROP_EXPRESSION, "PROPEXPRESSION", 0, "Expression", ""},  NOT_USED_IN_UI */
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "PropertySensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Property Sensor", "Sensor to detect values and changes in values of properties.");
	RNA_def_struct_sdna_from(srna, "bPropertySensor", "data");

	prop= RNA_def_property(srna, "evaluation_type", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "type");
	RNA_def_property_enum_items(prop, prop_type_items);
	RNA_def_property_ui_text(prop, "Evaluation Type", "Type of property evaluation.");

	prop= RNA_def_property(srna, "property", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "name");
	RNA_def_property_ui_text(prop, "Property", "");

	prop= RNA_def_property(srna, "value", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "value");
	RNA_def_property_ui_text(prop, "Value", "Check for this value in types in Equal or Not Equal types.");

	prop= RNA_def_property(srna, "min_value", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "value");
	RNA_def_property_ui_text(prop, "Minimum Value", "Specify minimum value in Interval type.");

	prop= RNA_def_property(srna, "max_value", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "maxvalue");
	RNA_def_property_ui_text(prop, "Maximum Value", "Specify maximum value in Interval type.");
}

static void rna_def_armature_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem prop_type_items[] ={
		{SENS_ARM_STATE_CHANGED, "STATECHG", 0, "State Changed", ""},
		{SENS_ARM_LIN_ERROR_BELOW, "LINERRORBELOW", 0, "Lin error below", ""},
		{SENS_ARM_LIN_ERROR_ABOVE, "LINERRORABOVE", 0, "Lin error above", ""},
		{SENS_ARM_ROT_ERROR_BELOW, "ROTERRORBELOW", 0, "Rot error below", ""},
		{SENS_ARM_ROT_ERROR_ABOVE, "ROTERRORBELOW", 0, "Rot error above", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "ArmatureSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Armature Sensor", "Sensor to detect values and changes in values of IK solver.");
	RNA_def_struct_sdna_from(srna, "bArmatureSensor", "data");

	prop= RNA_def_property(srna, "type", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "type");
	RNA_def_property_enum_items(prop, prop_type_items);
	RNA_def_property_ui_text(prop, "Test Type", "Type of value and test.");

	prop= RNA_def_property(srna, "channel_name", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "posechannel");
	RNA_def_property_ui_text(prop, "Bone name", "Identify the bone to check value from");

	prop= RNA_def_property(srna, "constraint_name", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "constraint");
	RNA_def_property_ui_text(prop, "Constraint name", "Identify the bone constraint to check value from.");

	prop= RNA_def_property(srna, "value", PROP_FLOAT, PROP_NONE);
	RNA_def_property_float_sdna(prop, NULL, "value");
	RNA_def_property_ui_text(prop, "Compare Value", "Specify value to be used in comparison.");
}

static void rna_def_actuator_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "ActuatorSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Actuator Sensor", "Sensor to detect state modifications of actuators.");
	RNA_def_struct_sdna_from(srna, "bActuatorSensor", "data");

	prop= RNA_def_property(srna, "actuator", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "name");
	RNA_def_property_ui_text(prop, "Actuator", "Actuator name, actuator active state modifications will be detected.");
}

static void rna_def_delay_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "DelaySensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Delay Sensor", "Sensor to send delayed events.");
	RNA_def_struct_sdna_from(srna, "bDelaySensor", "data");

	prop= RNA_def_property(srna, "delay", PROP_INT, PROP_NONE);
	RNA_def_property_ui_text(prop, "Delay", "Delay in number of logic tics before the positive trigger (default 60 per second).");
	RNA_def_property_range(prop, 0, 5000);

	prop= RNA_def_property(srna, "duration", PROP_INT, PROP_NONE);
	RNA_def_property_ui_text(prop, "Duration", "If >0, delay in number of logic tics before the negative trigger following the positive trigger.");
	RNA_def_property_range(prop, 0, 5000);

	prop= RNA_def_property(srna, "repeat", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "flag", SENS_DELAY_REPEAT);
	RNA_def_property_ui_text(prop, "Repeat", "Toggle repeat option. If selected, the sensor restarts after Delay+Dur logic tics.");
}

static void rna_def_collision_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem prop_type_items[] ={
		{0, "PROPERTY", 0, "Property", ""},
		{1, "MATERIAL", 0, "Material", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "CollisionSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Collision Sensor", "Sensor to detect objects colliding with the current object, with more settings than the Touch sensor.");
	RNA_def_struct_sdna_from(srna, "bCollisionSensor", "data");

	prop= RNA_def_property(srna, "property", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "name");
	RNA_def_property_ui_text(prop, "Property Name", "Only look for Objects with this property.");

	prop= RNA_def_property(srna, "material", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "materialName");
	RNA_def_property_ui_text(prop, "Material Name", "Only look for Objects with this material.");

	prop= RNA_def_property(srna, "collision_type", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "mode");
	RNA_def_property_enum_items(prop, prop_type_items);
	RNA_def_property_ui_text(prop, "Collision Type", "Toggle collision on material or property.");
}

static void rna_def_radar_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem axis_items[] ={
		{SENS_RAY_X_AXIS, "XAXIS", 0, "+X axis", ""},
		{SENS_RAY_Y_AXIS, "YAXIS", 0, "+Y axis", ""},
		{SENS_RAY_Z_AXIS, "ZAXIS", 0, "+Z axis", ""},
		{SENS_RAY_NEG_X_AXIS, "NEGXAXIS", 0, "-X axis", ""},
		{SENS_RAY_NEG_Y_AXIS, "NEGYAXIS", 0, "-Y axis", ""},
		{SENS_RAY_NEG_Z_AXIS, "NEGZAXIS", 0, "-Z axis", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "RadarSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Radar Sensor", "Sensor to detect objects in a cone shaped radar emanating from the current object.");
	RNA_def_struct_sdna_from(srna, "bRadarSensor", "data");

	prop= RNA_def_property(srna, "property", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "name");
	RNA_def_property_ui_text(prop, "Property", "Only look for Objects with this property.");

	prop= RNA_def_property(srna, "axis", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_items(prop, axis_items);
	RNA_def_property_ui_text(prop, "Axis", "Specify along which axis the radar cone is cast.");

	prop= RNA_def_property(srna, "angle", PROP_FLOAT, PROP_NONE);
	RNA_def_property_range(prop, 0.0, 179.9);
	RNA_def_property_ui_text(prop, "Angle", "Opening angle of the radar cone.");

	prop= RNA_def_property(srna, "distance", PROP_FLOAT, PROP_NONE);
	RNA_def_property_float_sdna(prop, NULL, "range");
	RNA_def_property_range(prop, 0.0, 10000.0);
	RNA_def_property_ui_text(prop, "Distance", "Depth of the radar cone.");
}

static void rna_def_random_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "RandomSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Random Sensor", "Sensor to send random events.");
	RNA_def_struct_sdna_from(srna, "bRandomSensor", "data");

	prop= RNA_def_property(srna, "seed", PROP_INT, PROP_NONE);
	RNA_def_property_range(prop, 0, 1000);
	RNA_def_property_ui_text(prop, "Seed", "Initial seed of the generator. (Choose 0 for not random).");
}

static void rna_def_ray_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;
	static EnumPropertyItem axis_items[] ={
		{SENS_RAY_X_AXIS, "XAXIS", 0, "+X axis", ""},
		{SENS_RAY_Y_AXIS, "YAXIS", 0, "+Y axis", ""},
		{SENS_RAY_Z_AXIS, "ZAXIS", 0, "+Z axis", ""},
		{SENS_RAY_NEG_X_AXIS, "NEGXAXIS", 0, "-X axis", ""},
		{SENS_RAY_NEG_Y_AXIS, "NEGYAXIS", 0, "-Y axis", ""},
		{SENS_RAY_NEG_Z_AXIS, "NEGZAXIS", 0, "-Z axis", ""},
		{0, NULL, 0, NULL, NULL}};
	static EnumPropertyItem prop_type_items[] ={
		{0, "PROPERTY", 0, "Property", ""},
		{1, "MATERIAL", 0, "Material", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "RaySensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Ray Sensor", "Sensor to detect intersections with a ray emanating from the current object.");
	RNA_def_struct_sdna_from(srna, "bRaySensor", "data");

	prop= RNA_def_property(srna, "property", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "propname");
	RNA_def_property_ui_text(prop, "Property", "Only look for Objects with this property.");

	prop= RNA_def_property(srna, "material", PROP_STRING, PROP_NONE);
	RNA_def_property_string_sdna(prop, NULL, "matname");
	RNA_def_property_ui_text(prop, "Material", "Only look for Objects with this material.");

	prop= RNA_def_property(srna, "ray_type", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "mode");
	RNA_def_property_enum_items(prop, prop_type_items);
	RNA_def_property_ui_text(prop, "Collision Type", "Toggle collision on material or property.");

	prop= RNA_def_property(srna, "x_ray_mode", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "mode", SENS_RAY_XRAY);
	RNA_def_property_ui_text(prop, "X-Ray Mode", "Toggle X-Ray option (see through objects that don't have the property).");

	prop= RNA_def_property(srna, "range", PROP_FLOAT, PROP_NONE);
	RNA_def_property_range(prop, 0.01, 10000.0);
	RNA_def_property_ui_text(prop, "Range", "Sense objects no farther than this distance.");

	prop= RNA_def_property(srna, "axis", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "axisflag");
	RNA_def_property_enum_items(prop, axis_items);
	RNA_def_property_ui_text(prop, "Axis", "Specify along which axis the ray is cast.");
}

static void rna_def_message_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	srna= RNA_def_struct(brna, "MessageSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Message Sensor", "Sensor to detect incoming messages.");
	RNA_def_struct_sdna_from(srna, "bMessageSensor", "data");

	prop= RNA_def_property(srna, "subject", PROP_STRING, PROP_NONE);
	RNA_def_property_ui_text(prop, "Subject", "Optional subject filter: only accept messages with this subject, or empty for all.");
}

static void rna_def_joystick_sensor(BlenderRNA *brna)
{
	StructRNA *srna;
	PropertyRNA *prop;

	static EnumPropertyItem event_type_items[] ={
		{SENS_JOY_BUTTON, "BUTTON", 0, "Button", ""},
		{SENS_JOY_AXIS, "AXIS", 0, "Axis", ""},
		{SENS_JOY_HAT, "HAT", 0, "Hat", ""},
		{0, NULL, 0, NULL, NULL}};

	static EnumPropertyItem axis_direction_items[] ={
		{SENS_JOY_X_AXIS, "RIGHTAXIS", 0, "Right Axis", ""},
		{SENS_JOY_Y_AXIS, "UPAXIS", 0, "Up Axis", ""},
		{SENS_JOY_NEG_X_AXIS, "LEFTAXIS", 0, "Left Axis", ""},
		{SENS_JOY_NEG_Y_AXIS, "DOWNAXIS", 0, "Down Axis", ""},
		{0, NULL, 0, NULL, NULL}};

	srna= RNA_def_struct(brna, "JoystickSensor", "Sensor");
	RNA_def_struct_ui_text(srna, "Joystick Sensor", "Sensor to detect joystick events.");
	RNA_def_struct_sdna_from(srna, "bJoystickSensor", "data");
	
	prop= RNA_def_property(srna, "joystick_index", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "joyindex");
	RNA_def_property_ui_text(prop, "Joystick Index", "Specify which joystick to use.");
	RNA_def_property_range(prop, 0, SENS_JOY_MAXINDEX-1);

	prop= RNA_def_property(srna, "event_type", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "type");
	RNA_def_property_enum_items(prop, event_type_items);
	RNA_def_property_ui_text(prop, "Event Type", "The type of event this joystick sensor is triggered on.");

	prop= RNA_def_property(srna, "all_events", PROP_BOOLEAN, PROP_NONE);
	RNA_def_property_boolean_sdna(prop, NULL, "flag", SENS_JOY_ANY_EVENT);
	RNA_def_property_ui_text(prop, "All Events", "Triggered by all events on this joysticks current type (axis/button/hat).");

	/* Button */
	prop= RNA_def_property(srna, "button_number", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "button");
	RNA_def_property_ui_text(prop, "Button Number", "Specify which button to use.");
	RNA_def_property_range(prop, 0, 18);

	/* Axis */
	prop= RNA_def_property(srna, "axis_number", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "axis");
	RNA_def_property_ui_text(prop, "Axis Number", "Specify which axis pair to use, 1 is usually the main direction input.");
	RNA_def_property_range(prop, 1, 2);

	prop= RNA_def_property(srna, "axis_threshold", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "precision");
	RNA_def_property_ui_text(prop, "Axis Threshold", "Specify the precision of the axis.");
	RNA_def_property_range(prop, 0, 32768);

	prop= RNA_def_property(srna, "axis_direction", PROP_ENUM, PROP_NONE);
	RNA_def_property_enum_sdna(prop, NULL, "axisf");
	RNA_def_property_enum_items(prop, axis_direction_items);
	RNA_def_property_ui_text(prop, "Axis Direction", "The direction of the axis.");

	/* Hat */
	prop= RNA_def_property(srna, "hat_number", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "hat");
	RNA_def_property_ui_text(prop, "Hat Number", "Specify which hat to use.");
	RNA_def_property_range(prop, 1, 2);

	prop= RNA_def_property(srna, "hat_direction", PROP_INT, PROP_NONE);
	RNA_def_property_int_sdna(prop, NULL, "hatf");
	RNA_def_property_ui_text(prop, "Hat Direction", "Specify hat direction.");
	RNA_def_property_range(prop, 0, 12);
}

void RNA_def_sensor(BlenderRNA *brna)
{
	rna_def_sensor(brna);

	rna_def_always_sensor(brna);
	rna_def_near_sensor(brna);
	rna_def_mouse_sensor(brna);
	rna_def_touch_sensor(brna);
	rna_def_keyboard_sensor(brna);
	rna_def_property_sensor(brna);
	rna_def_armature_sensor(brna);
	rna_def_actuator_sensor(brna);
	rna_def_delay_sensor(brna);
	rna_def_collision_sensor(brna);
	rna_def_radar_sensor(brna);
	rna_def_random_sensor(brna);
	rna_def_ray_sensor(brna);
	rna_def_message_sensor(brna);
	rna_def_joystick_sensor(brna);
}

#endif

