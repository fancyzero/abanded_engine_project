-- export by sssa2000
macroScript ExportGeo category:"HowTo2"
(
    fn GetGeometry o = 
    (
    
    Superclassof o == Geometryclass and classof o != TargetObject 
    )

    fn DumpMaterial m file=
    (
    --只对Standard类型的材质处理
    --获得diffuse and diffuse map
        ismat=iskindof m material 
        if ismat then
        (
            
            name=m.name
            format "Material Name-> %\n" name to: file
            
            class_of_mat=classof m
            --messagebox class_of_mat
            if (class_of_mat )==Standardmaterial then
            (
                
            )
        )
        format "\n" to: file
    )
    --/////////////////////////////////////////////////////////////////////////////////////////
	print "fdasfas"
    obj = pickobject filter:GetGeometry
    
    if isValidNode obj then
    (
        nodename=obj.name
		print nodename
        --First export the matrix
        row1=obj.transform.row1
        row2=obj.transform.row2
        row3=obj.transform.row3
        row4=obj.transform.row4
        
        tmesh = snapshotAsMesh obj 
        out_name = GetSaveFileName()
        if out_name != undefined then
        (
            out_file = createfile out_name

            format "Node Name: %\n" nodename to: out_file
            format "\n" to: out_file
            --******************************************************
            -- material info
            node_material=obj.material
            num_sub_material=getNumSubMtls obj.material
            DumpMaterial obj.material out_file
            for y=1 to num_sub_material do
            (
                sub_mat=getSubMtl obj.material y
               -- DumpMaterial sub_mat= out_file
            )
            --******************************************************

            format "Node TM Row1: %\n" row1 to: out_file
            format "Node TM Row2: %\n" row2 to: out_file
            format "Node TM Row3: %\n" row3 to: out_file
            format "Node TM Row4: %\n" row4 to: out_file
            --******************************************************
            -- vertex info
            num_verts = tmesh.numverts 
            num_faces = tmesh.numfaces
            format "Number of Ver: %\n" num_verts to:out_file
            for v = 1 to num_verts do
                format "Ver%: %\n" v (getVert tmesh v) to:out_file
            format "\n" to:out_file
            --***********************************************
            --face info
            format "Number of Face: %\n" num_faces to:out_file
            for f = 1 to num_faces do
            (
                face = getFace tmesh f
                sgroup = getFaceSmoothGroup tmesh f
                matid = getFaceMatId tmesh f
                edge1 = getEdgeVis tmesh f 1
                edge2 = getEdgeVis tmesh f 2
                edge3 = getEdgeVis tmesh f 3
                format "%,%,%,%,%,%\n" face sgroup matid edge1 edge2 edge3 to:out_file
            )
            --******************************************************
            --uv info
            channel=meshop.getnummaps tmesh --number of texture
            format "\n" to: out_file
            format "Channel Number:%\n" channel to:out_file
            for i=1 to (channel-1) do
            (
                
                -- channel的计数从0开始
                --channel 0 is vertex color so do not export it
                IsSupport=meshop.getMapSupport tmesh i
                if IsSupport==true then
                (
                    format "Channel ID:%\n" i to:out_file
                    if classof obj.material.maps[i+1]==Bitmaptexture then
                        format "Map File Path->%\n" obj.material.maps[i+1].filename to: out_file
                    else
                        format "Map File Path->Null\n" to: out_file
                    num_uv_ver=meshop.getNumMapVerts tmesh i
                    num_uv_face=meshop.getNumMapFaces tmesh i
                    format "UVW Vertex Number:%\n" num_uv_ver to:out_file
                    for j=1 to num_uv_ver do
                    (
                        vert_uvw=meshop.getMapVert tmesh i j 
                        --messagebox (vert_uvw as string)
                        format "% \n" vert_uvw to: out_file
                    )
                    format "UVW Face Number:%\n" num_uv_face to:out_file
                    for o=1 to num_uv_face do
                    (
                        uvw_face=meshop.getMapFace tmesh i o
                        format "% \n" uvw_face to: out_file
                    )
                )
                else
                (
                    --format "Do Not Support Channel %\n" i to:out_file
                )
            )
        close out_file
        edit out_name
        )
    
    )

)