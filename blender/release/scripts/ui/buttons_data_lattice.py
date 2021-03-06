
import bpy

class DataButtonsPanel(bpy.types.Panel):
	__space_type__ = 'PROPERTIES'
	__region_type__ = 'WINDOW'
	__context__ = "data"
	
	def poll(self, context):
		return context.lattice
	
class DATA_PT_context_lattice(DataButtonsPanel):
	__show_header__ = False
	
	def draw(self, context):
		layout = self.layout
		
		ob = context.object
		lat = context.lattice
		space = context.space_data

		split = layout.split(percentage=0.65)

		if ob:
			split.template_ID(ob, "data")
			split.itemS()
		elif lat:
			split.template_ID(space, "pin_id")
			split.itemS()

class DATA_PT_lattice(DataButtonsPanel):
	__label__ = "Lattice"

	def draw(self, context):
		layout = self.layout
		
		lat = context.lattice

		row = layout.row()
		row.itemR(lat, "points_u")
		row.itemR(lat, "interpolation_type_u", expand=True)
			
		row = layout.row()
		row.itemR(lat, "points_v")
		row.itemR(lat, "interpolation_type_v", expand=True)
			
		row = layout.row()
		row.itemR(lat, "points_w")
		row.itemR(lat, "interpolation_type_w", expand=True)
			
		row = layout.row()
		row.itemO("lattice.make_regular")
		row.itemR(lat, "outside")

bpy.types.register(DATA_PT_context_lattice)
bpy.types.register(DATA_PT_lattice)
