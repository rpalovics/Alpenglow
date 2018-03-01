import alpenglow.Getter as rs
import alpenglow as prs


class NearestNeighborExperiment(prs.OnlineExperiment):
    """NearestNeighborExperiment(gamma=0.8,direction="forward",gamma_threshold=0,num_of_neighbors=10)

    This class implements an online version of a similarity based recommendation model.
    One of the earliest and most popular collaborative filtering algorithms in practice is the item-based nearest neighbor [Sarwar2001]_
    For these algorithms similarity scores are computed between item pairs based on the co-occurrence of the pairs in the preference of users. Non-stationarity of the data can be accounted for e.g. with the introduction of a time-decay [Ding2005]_ .

    Describing the algorithm more formally, let us denote by :math:`U_i` the set of users that visited item :math:`i`, by :math:`I_u` the set of items visited by user :math:`u`, and by :math:`s_{u i}` the index of item :math:`i` in the sequence of interactions of user :math:`u`. The frequency based time-weighted similarity function is defined by :math:`sim(j,i) = \\frac{\sum_{u\in {U_j \cap U_i}} f(s_{ui} - s_{uj})}{\left|U_j\\right|}`, where :math:`f(\\tau)=\gamma^\\tau` is the time decaying function. For non-stationary data we sum only over users that visit item :math:`j` before item :math:`i`, setting :math:`f(\\tau)=0` if :math:`\\tau < 0`. For stationary data the absolute value of :math:`\\tau` is used. The score assigned to item :math:`i` for user :math:`u` is :math:`score(u,i) = \sum_{j\in{I_u}} f\left(\left| I_u \\right| - s_{uj}\\right) sim(j,i).`
    The model is represented by the similarity scores. Since computing the model is time consuming, it is done periodically. Moreover, only the most similar items are stored for each item. When the prediction scores are computed for a particular user, all items visited by the user can be considered, including the most recent ones. Hence, the algorithm can be considered semi-online in that it uses the most recent interactions of the current user, but not of the other users. We note that the time decay function is used here to quantify the strength of connection between pairs of items depending on how closely are located in the sequence of a user, and not as a way to forget old data as in [Ding2005]_.

    .. [Sarwar2001] B. Sarwar, G. Karypis, J. Konstan, and J. Reidl. Item-based collaborative filtering recommendation algorithms. In Proc. WWW, pages 285–295, 2001.
    .. [Ding2005] Y. Ding and X. Li. Time weight collaborative filtering. In Proc. CIKM, pages 485–492. ACM, 2005.

    Parameters
    ----------
    gamma : double
        The constant used in the decay function. It shoud be set to 1 in offline and stationary experiments.
    direction : string
        Set to "forward" to consider the order of item pairs. Set to "both" when the order is not relevant.
    gamma_thresold : double
        Threshold to omit very small members when summing similarity.  If the value of the decay function is smaller than the threshold, we omit the following members.  Defaults to 0 (do not omit small members).
    num_of_neighbors : int
        The number of most similar items that will be stored in the model.
    """
    def _config(self, top_k, seed):
        model = rs.NearestNeighborModel(**self.parameter_defaults(
          gamma=0.8,
          norm="num",
          direction="forward",
          gamma_threshold=0,
          num_of_neighbors=10
        ))
        updater = rs.NearestNeighborModelUpdater(**self.parameter_defaults(
          compute_similarity_period=86400,
          period_mode="time-based"
        ))
        updater.set_model(model)

        return (model, updater, [], [])
