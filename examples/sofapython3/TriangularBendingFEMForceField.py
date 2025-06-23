def createScene(rootNode):

    rootNode.addObject('RequiredPlugin', name="Shell")
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Projective') # Needed to use components [FixedProjectiveConstraint]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Engine.Select') # Needed to use components [BoxROI]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.IO.Mesh') # Needed to use components [MeshOBJLoader]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct') # Needed to use components [SparseLDLSolver]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Mapping.Linear') # Needed to use components [IdentityMapping]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Mass') # Needed to use components [UniformMass]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward') # Needed to use components [EulerImplicitSolver]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.StateContainer') # Needed to use components [MechanicalObject]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Topology.Container.Constant') # Needed to use components [MeshTopology]  
    rootNode.addObject('RequiredPlugin', name='Sofa.GL.Component.Rendering3D') # Needed to use components [OglModel]  
    rootNode.addObject('RequiredPlugin', name='Sofa.GUI.Component') # Needed to use components [AttachBodyButtonSetting]  

    rootNode.gravity = [0, -98.1, 0]
    rootNode.addObject('AttachBodyButtonSetting', stiffness=0.1)
    rootNode.addObject('DefaultAnimationLoop')

    square = rootNode.addChild('Square')
    square.addObject('EulerImplicitSolver')
    square.addObject('SparseLDLSolver')
    square.addObject('MeshOBJLoader', filename='mesh/square1.obj')
    square.addObject('MeshTopology', src=square.MeshOBJLoader.getLinkPath())
    square.addObject('MechanicalObject', template='Rigid3')
    square.addObject('UniformMass', totalMass=0.005)
    square.addObject('BoxROI', box=[0, 0.9, -0.1, 1, 1, 0.1], drawBoxes=True)
    square.addObject('FixedProjectiveConstraint', indices=square.BoxROI.indices.getLinkPath())
    square.addObject('TriangularBendingFEMForceField', youngModulus=1.7e3,
                     poissonRatio=0.3, thickness=0.01)

    visu = square.addChild('Visu')
    visu.addObject('OglModel', src=square.MeshTopology.getLinkPath())
    visu.addObject('IdentityMapping')

