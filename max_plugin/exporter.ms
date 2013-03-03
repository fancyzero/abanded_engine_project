--导出选中的模型，每个object为一个三角形列表，最后导出一份材质列表
struct vertex ( pos, uv )
struct face ( v=#(), normal=[0,0,0] )

function dump_mesh in_mesh faces =
(
	faces = #()
	max_channel = (meshop.getNumMaps in_mesh)
	print max_channel
	--很妖怪，uvchannel从0开始，channel 0 是vertexcolor 所以跳过，
	num_verts = (getnumverts in_mesh)
	num_faces = (getnumfaces in_mesh)
	for i = 1 to (max_channel-1) do 
	(
		is_supported = meshop.getMapSupport in_mesh i
		if is_supported then
		(
			
			num_uv_face=meshop.getNumMapFaces in_mesh i
			if ( num_faces != num_uv_face ) then
				messagebox "error!"
			format "total faces %\n" num_uv_face
			for face_idx =1 to num_uv_face do
			(
				uv_face = meshop.getMapFace in_mesh i face_idx
				mesh_face = ( getface in_mesh face_idx )
				--format "uvface % mesh_face %\n" uv_face  mesh_face
				
				f = face ()
				f.v = #()
				
				for n=1 to 3 do
				(
					f.v[n] = vertex (mesh_face[n] as Integer) #(uv_face[n] as Integer)
														
				)
				f.normal = meshop.getFaceRNormals in_mesh face_idx
				faces[faces.count+1] = f
			)			
		)
	)
)

function dump_material obj stream =
(
)

outf = createfile "g:\\temp\\out_mesh.txt"
format "<xml>\n" to:outf
for obj in selection do
(
	faces = #()	
	
	format "<object>\n"  to:outf
	format "<name>%</name>\n" obj.name to:outf
	format "<transform>% % % % % % % % % % % %</transform>\n" \
			obj.transform.row1.x obj.transform.row1.y obj.transform.row1.z\
			obj.transform.row2.x obj.transform.row2.y obj.transform.row2.z\
			obj.transform.row3.x obj.transform.row3.y obj.transform.row3.z\
			obj.transform.row4.x obj.transform.row4.y obj.transform.row4.z
	
	format "<mesh_vertex_array>\n" to:outf
	format "<count>%</count>\n<data>\n" (meshop.getnumverts obj) to:outf
	num_mesh_verts = meshop.getnumverts obj
	for i = 1 to num_mesh_verts do
	(
		vert = meshop.getvert obj i
		format "% % % " vert.x vert.y vert.z to:outf
	)
	format "\n</data>\n</mesh_vertex_array>\n" to:outf
	format "<faces>\n" to:outf
	--format "<count>%</count>\n" faces.count to:outf
	
	dump_mesh obj outf;
	
	--for i=1 to faces.count do
	--(
		--format "<face%>\n" (i-1)	to:outf
		--for j=1 to faces[i].v.count do
		--(
			--format "<vertex%>\n" (j-1) to:outf
			--format "<pos>%</pos>\n" faces[i].v[j].pos to:outf
			--format "<uvs><count>%</count>" faces[i].v[j].uv.count to:outf
			--for k=1 to faces[i].v[j].uv.count do
			--(
				--format "<uv%>%</uv%>" (k-1) faces[i].v[j].uv[k] (k-1) to:outf
			--)
			--format "\n</uvs>\n" to:outf
			--format "</vertex%>\n" (j-1) to:outf
		--)
		--format "</face%>\n" (i-1) to:outf
	--)
	--format "</faces>\n" to:outf
	--format "</object>\n" to:outf
)
format "</xml>\n" to:outf
close outf	


