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

    function T = subsref(obj, index)
      arguments
        obj Mesh
        index uint64
      end
      pts = obj.points;
      con = obj.connectivity;
      T = Triangle(pts(con(index,1)), pts(con(index,2)), pts(con(index,3)));
    end

    function A = area(obj)
      A = 0;
      for i=1:size(obj.connectivity,1)
        T = obj.subsref(i);
        A = A + T.area;
      end
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
        loc = find(obj.points == pts(i));
        if isempty(loc)
          obj.points = [obj.points; pts(i)];
          obj.connectivity(end, i) = size(obj.points, 1);
        else
          obj.connectivity(end, i) = loc;
        end
      end
    end
  end
end