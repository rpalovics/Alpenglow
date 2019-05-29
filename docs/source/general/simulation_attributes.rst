Adjustable global properties of the online experiment
=====================================================

When running an online experiment, the user can control some modeling decisions and the flow of the experiment through global parameters.  These modeling decisions, control options and the parameters are described below.

The components in the online experiment can obtain the value of these parameters through query functions of :py:class:`alpenglow.cpp.ExperimentEnvironment`.  Note that some models ignore the value set in the common parameter container and always use the default value or a locally set value.

+---------------------------------+---------------------------------------------+
| parameter name                  | description                                 |
+=================================+=============================================+
| ``exclude_known``               | Excludes items from evaluation that the     |
|                                 | actual user already interacted with.        |
|                                 | Besides evaluation, influences negative     |
|                                 | sample generation in gradient trainting.    |
+---------------------------------+---------------------------------------------+
| ``initialize_all``              | Set true to treat all users and items as    |
|                                 | existing from the beginning of the          |
|                                 | experiment.  Technically, the largest user  |
|                                 | and item is searched in the time series and |
|                                 | all ids starting from 0 will be treated as  |
|                                 | existing. By default this parameters is set |
|                                 | to false, meaning that users and items      |
|                                 | come into existence by their first          |
|                                 | occurrence in a training sample.            |
+---------------------------------+---------------------------------------------+
| ``top_k``                       | Sets the toplist length.  Models may treat  |
|                                 | scores liberally that belong to items that  |
|                                 | are under the limit to optimize running     |
|                                 | time.                                       |
+---------------------------------+---------------------------------------------+
| ``evaluation_start_time``       | Do not evaluate samples having smaller      |
|                                 | timestamp.                                  |
+---------------------------------+---------------------------------------------+
| ``experiment_termination_time`` | Terminates experiment after the first       |
|                                 | sample having equal or larger time stamp.   |
+---------------------------------+---------------------------------------------+

TODO: usage examples, sample results, exclude_known and eval_start_time
