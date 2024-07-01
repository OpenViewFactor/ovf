classdef Mesh < handle
  properties
    points (:,1) Vector3
    connectivity (:,3) uint64
    name MeshType
  end

  methods
    function obj = Mesh(~)
    end

    function name = getName(obj)
      name = obj.name;
    end
    
    function num_elements = numElements(obj)
      num_elements = size(obj.connectivity, 1);
    end
    function num_points = numPoints(obj)
      num_points = size(obj.points, 1);
    end

    function setName(obj, name)
      arguments
        obj Mesh
        name MeshType
      end
      obj.name = name;
    end
    function addElement(obj, OA, OB, OC)
      arguments
        obj Mesh
        OA Vector3
        OB Vector3
        OC Vector3
      end
      pts = [OA, OB, OC];
      obj.connectivity = [obj.connectivity; 0,0,0];
      for i=1:length(pts)
        if ~isempty(obj.points)
          loc = find(obj.points == pts(i));
          if isempty(loc)
            obj.points = [obj.points; pts(i)];
            obj.connectivity(end, i) = size(obj.points, 1);
          else
            obj.connectivity(end, i) = loc;
          end
        else
          obj.points = [obj.points, pts(i)];
          obj.connectivity(end, i) = i;
        end
      end
    end

    function T = getElement(obj, index)
      arguments
        obj Mesh
        index uint64
      end
      pts = obj.points;
      con = obj.connectivity;
      T = Triangle(pts(con(index,1)), pts(con(index,2)), pts(con(index,3)));
    end

    function sub_mesh = getSubMesh(obj, indices)
      arguments
        obj Mesh
        indices (1,:) uint64
      end
      sub_mesh = Mesh;
      sub_mesh.setConnectivity(obj.connectivity(indices,:));
      sub_mesh.setPoints(obj.points);
    end
    function setConnectivity(obj, con)
      arguments
        obj Mesh
        con (:,3) uint64
      end
      obj.connectivity = con;
    end
    function setPoints(obj, pts)
      arguments
        obj Mesh
        pts (:,1) Vector3
      end
      obj.points = pts;
    end

    function swapElements(obj, index_one, index_two)
      arguments
        obj Mesh
        index_one (1,1) uint64
        index_two (1,1) uint64
      end
      t1_connectivity = obj.connectivity(index_one, :);
      t2_connectivity = obj.connectivity(index_two, :);
      obj.connectivity(index_two, :) = t1_connectivity;
      obj.connectivity(index_one, :) = t2_connectivity;
    end

    function centroids = getCentroids(obj)
      centroids = Vector3.empty(size(obj.connectivity, 1), 0);
      for i=1:size(obj.connectivity,1)
        T = obj.getElement(i);
        centroids(i) = T.getCentroid;
      end
    end

    function normals = getNormals(obj)
      normals = Vector3.empty(size(obj.connectivity, 1), 0);
      for i=1:size(obj.connectivity,1)
        T = obj.getElement(i);
        normals(i) = T.getNormal;
      end
    end

    function A = area(obj)
      A = 0;
      for i=1:size(obj.connectivity,1)
        T = obj.getElement(i);
        A = A + T.getArea;
      end
    end

    function plotMesh(obj)
      double_points = zeros(obj.numPoints, 3, "double");
      vector3_points = obj.points;
      for i = 1 : obj.numPoints
        double_points(i, :) = [vector3_points(i).getX, vector3_points(i).getY, vector3_points(i).getZ];
      end
      matlab_triangulation = triangulation(double(obj.connectivity), double_points);
      trisurf(matlab_triangulation)
      daspect([1,1,1])
      view(3)
    end
  end
end