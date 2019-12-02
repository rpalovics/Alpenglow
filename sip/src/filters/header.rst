Filters
-------

The function of the filter interface is limiting the available set of items.  Current filters are whitelist-type filters, implementing :py:class:`alpenglow.cpp.WhitelistFilter`.

To use a filter in an experiment, wrap the model into the filter using :py:class:`alpenglow.cpp.WhitelistFilter2ModelAdapter`.

Example:

.. code-block:: python
  :emphasize-lines: 8-13

  class LabelExperiment(prs.OnlineExperiment):
      '''Sample experiment illustrating the usage of LabelFilter. The
      experiment contains a PopularityModel and a LabelFilter.'''
      def _config(self, top_k, seed):
          model = ag.PopularityModel()
          updater = ag.PopularityModelUpdater()
          updater.set_model(model)
          label_filter = ag.LabelFilter(**self.parameter_defaults(
              label_file_name = ""
          ))
          adapter = ag.WhitelistFilter2ModelAdapter()
          adapter.set_model(model)
          adapter.set_whitelist_filter(label_filter)
