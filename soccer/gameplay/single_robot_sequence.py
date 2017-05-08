import behavior_sequence
import single_robot_composite_behavior
import role_assignment

#A sequence to be carried out by a single robot, attempting to add non-single robot behaviors will raise exceptons
class SingleRobotSequence(behavior_sequence.BehaviorSequence, single_robot_composite_behavior.SingleRobotCompositeBehavior):
    def __init__(self, behaviors = [], continuous=False, autorestart=lambda: True):
        single_robot_composite_behavior.SingleRobotCompositeBehavior.__init__(self, continuous, autorestart)
        behavior_sequence.BehaviorSequence.__init__(self, behaviors = behaviors)
 
    def role_requirements(self):
       #forces all subbhehaviors to use the same robot
        if len(self.behaviors) > 0 and self.robot != None:
            reqs = behavior_sequence.BehaviorSequence.role_requirements(self)
            for req in role_assignment.iterate_role_requirements_tree_leaves(reqs):
                req.required_shell_id = self.robot.shell_id()
        else:
            reqs =  single_robot_composite_behavior.SingleRobotCompositeBehavior.role_requirements(self)
        return reqs
