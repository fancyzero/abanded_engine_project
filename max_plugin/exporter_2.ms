--导出选中的模型，每个object为一个三角形列表，最后导出一份材质列表
struct vertex ( pos, uv )
struct face ( v=#(), normal=[0,0,0] )

function dump_mesh in_mesh outf =
(
	max_channel = (meshop.getNumMaps in_mesh)
	print max_channel
	--很妖怪，uvchannel从0开始，channel 0 是vertexcolor 所以跳过，
	num_verts = (getnumverts in_mesh)
	num_faces = (getnumfaces in_mesh)
	--for i = 1 to (max_channel-1) do 
	i = 1
	--(
		--is_supported = meshop.getMapSupport in_mesh i
		--if is_supported then
		--(
			
			num_uv_face=meshop.getNumMapFaces in_mesh i
			--if ( num_faces != num_uv_face ) then
				--messagebox "error!"
			--format "total faces %\n" num_uv_face
			for face_idx =1 to num_uv_face do
			(
				--if ( mod face_idx (num_uv_face/100) == 0 ) then
					--format "progress: %\%, current face: %\n" ((face_idx/num_uv_face as float)*100) face_idx
				--uv_face = meshop.getMapFace in_mesh i face_idx
				--mesh_face = ( getface in_mesh face_idx )
				--format "uvface % mesh_face %\n" uv_face  mesh_face
				
				--f = face ()
				--f.v = #()
				
				--for n=1 to 3 do
				--(
					--f.v[n] = vertex (mesh_face[n] as Integer) #(uv_face[n] as Integer)
														
				--)
				--f.normal = meshop.getFaceRNormals in_mesh face_idx
				--faces[faces.count+1] = f
				--format "<face%>\n" (face_idx-1)	to:outf
				map_face = meshop.getmapface in_mesh 1 face_idx
				
				--for vert_id = 1 to 3 do
				(
					writelong outf (map_face.x as integer)
					writelong outf (map_face.y as integer)
					writelong outf (map_face.z as integer)
					--format "<vertex%>\n" (vert_id-1) to:outf
					--format "<pos>%</pos>\n" f.v[vert_id].pos to:outf
					--format "<uvs>\n<count>%</count>" f.v[vert_id].uv.count to:outf
					--for k=1 to f.v[vert_id].uv.count do
					--(
						--format "<uv%>%</uv%>" (k-1) f.v[vert_id].uv[k] (k-1) to:outf
					--)
					--format "\n</uvs>\n" to:outf
					--format "</vertex%>\n" (vert_id-1) to:outf
				)
				--format "</face%>\n" (face_idx-1) to:outf
			)			
		--)
	--)
)

function dump_material obj stream =
(
)
start = timeStamp()


outf = createfile "g:\\temp\\out_mesh.txt"
close outf
outf = fopen "g:\\temp\\out_mesh.txt" "wb"
--format "<xml>\n" to:outf
for obj in selection do
(
	faces = #()	
	
	--format "<object>\n"  to:outf
	--format "<name>%</name>\n" obj.name to:outf
	--format "<transform>% % % % % % % % % % % %</transform>\n" \
		--	obj.transform.row1.x obj.transform.row1.y obj.transform.row1.z\
			--obj.transform.row2.x obj.transform.row2.y obj.transform.row2.z\
			--obj.transform.row3.x obj.transform.row3.y obj.transform.row3.z\
			--obj.transform.row4.x obj.transform.row4.y obj.transform.row4.z\
			--to:outf
	--format "<mesh_vertex_array>\n" to:outf
	--format "<count>%</count>\n<data>\n" (meshop.getnumverts obj) to:outf
	num_mesh_verts = meshop.getnumverts obj
	for i = 1 to num_mesh_verts do
	(
		vert = meshop.getvert obj i
		writefloat outf vert.x
		writefloat outf vert.y
		writefloat outf vert.z
		--format "% % % " vert.x vert.y vert.z to:outf
	)
	--format "\n</data>\n</mesh_vertex_array>\n" to:outf
	--format "<faces>\n" to:outf
	--format "<count>%</count>\n" faces.count to:outf
	
	dump_mesh obj outf;
	
	--format "</faces>\n" to:outf
	--format "</object>\n" to:outf
)
--format "</xml>\n" to:outf
fclose outf	
end = timeStamp()
format "Processing took % seconds\n" ((end - start) / 1000.0)


