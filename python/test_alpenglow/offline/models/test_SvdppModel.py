import alpenglow as prs
from alpenglow.offline.models import SvdppModel
import alpenglow.Getter as rs
import pandas as pd
import numpy as np
import unittest


class TestSvdppModel(unittest.TestCase):
    def test_rmse(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        model = SvdppModel(
            negative_rate=9,
            number_of_iterations=20,
        )
        model.fit(data)

        def predict(model, user, item):
            rd = rs.RecDat()
            rd.user = user
            rd.item = item
            return model.prediction(rd)

        errors = [(1 - predict(model.model, u, i))**2 for (u, i) in data[['user', 'item']].values]
        rmse = np.sqrt(pd.Series(errors)).mean()
        self.assertAlmostEqual(0.4043016468150385, rmse)

    def test_ranking(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        exp = SvdppModel(
            negative_rate=9,
            number_of_iterations=20,
        )
        exp.fit(data)
        preds = exp.recommend()
        print("toplist="+str(preds['item'].tolist()))
        assert preds['item'].tolist() ==  \
            [94, 225, 30, 166, 300, 299, 442, 196, 337, 372, 250, 455, 462, 204, 215, 40, 62, 429, 496, 38, 247, 36, 338, 255, 427, 165, 256, 293, 128, 211, 177, 156, 86, 483, 102, 371, 375, 444, 292, 25, 450, 383, 120, 266, 108, 251, 497, 97, 491, 414, 434, 282, 468, 81, 400, 452, 263, 4, 181, 168, 277, 197, 271, 99, 69, 298, 325, 295, 421, 16, 351, 395, 479, 330, 464, 161, 54, 5, 425, 216, 22, 245, 436, 440, 458, 206, 403, 127, 195, 146, 205, 118, 348, 17, 191, 318, 281, 105, 29, 284, 300, 204, 299, 165, 86, 215, 197, 452, 427, 429, 250, 483, 16, 40, 81, 497, 116, 29, 400, 468, 266, 371, 403, 6, 440, 436, 205, 318, 496, 330, 434, 234, 450, 356, 251, 108, 95, 195, 488, 296, 233, 491, 255, 58, 454, 188, 281, 284, 421, 295, 271, 246, 314, 414, 15, 99, 425, 498, 192, 445, 12, 298, 142, 273, 410, 93, 380, 1, 348, 254, 474, 146, 206, 301, 252, 481, 294, 33, 139, 406, 7, 157, 270, 407, 268, 362, 409, 23, 178, 122, 35, 499, 484, 485, 361, 231, 353, 302, 72, 237, 166, 225, 30, 299, 442, 337, 196, 215, 250, 40, 36, 62, 38, 247, 204, 293, 256, 211, 444, 483, 165, 292, 25, 120, 128, 177, 156, 97, 414, 251, 86, 434, 491, 102, 383, 375, 400, 450, 81, 181, 271, 263, 108, 266, 99, 325, 452, 4, 330, 168, 282, 298, 421, 351, 464, 277, 69, 205, 479, 197, 395, 425, 348, 146, 206, 440, 281, 314, 216, 22, 192, 195, 93, 127, 245, 16, 12, 118, 161, 191, 488, 478, 436, 353, 96, 54, 122, 403, 15, 174, 188, 454, 5, 233, 246, 318, 17, 132, 397, 356, 30, 225, 166, 94, 299, 372, 337, 98, 215, 300, 36, 496, 462, 338, 40, 196, 250, 429, 62, 255, 455, 444, 38, 256, 120, 211, 292, 427, 483, 293, 414, 247, 97, 25, 434, 156, 181, 491, 204, 177, 251, 371, 165, 128, 383, 295, 400, 263, 271, 81, 99, 102, 86, 168, 4, 351, 325, 497, 375, 348, 330, 452, 450, 298, 205, 395, 464, 479, 277, 108, 281, 421, 440, 105, 69, 266, 425, 192, 314, 96, 93, 206, 78, 188, 246, 12, 174, 112, 146, 127, 162, 498, 410, 353, 197, 419, 494, 195, 53, 23, 300, 94, 225, 30, 166, 98, 204, 337, 442, 165, 250, 455, 299, 38, 62, 247, 128, 86, 427, 372, 462, 293, 266, 429, 156, 450, 375, 102, 177, 108, 215, 40, 256, 496, 255, 371, 483, 211, 468, 36, 25, 383, 497, 4, 292, 197, 251, 282, 338, 16, 452, 298, 395, 400, 421, 436, 81, 277, 491, 69, 263, 97, 403, 245, 168, 216, 444, 284, 22, 99, 318, 118, 161, 6, 54, 234, 58, 191, 356, 325, 296, 434, 464, 80, 236, 351, 95, 278, 181, 221, 440, 5, 271, 414, 458, 425, 127, 195, 120, 17, 196, 225, 30, 166, 204, 98, 337, 247, 462, 455, 372, 102, 256, 250, 299, 375, 38, 62, 429, 177, 427, 156, 266, 450, 215, 497, 483, 108, 468, 282, 371, 452, 40, 211, 197, 36, 69, 16, 496, 251, 81, 255, 263, 4, 168, 338, 161, 54, 277, 400, 5, 395, 458, 298, 479, 403, 25, 245, 284, 216, 6, 318, 29, 195, 22, 116, 234, 118, 58, 17, 356, 296, 191, 377, 95, 265, 188, 488, 397, 289, 325, 440, 464, 77, 132, 80, 113, 236, 278, 498, 292, 246, 491, 410, 97, 233, 221, 434, 273, 454, 94, 225, 30, 166, 204, 98, 337, 442, 455, 247, 299, 128, 86, 372, 250, 102, 38, 62, 256, 429, 375, 427, 383, 177, 215, 156, 266, 293, 450, 40, 483, 371, 211, 36, 108, 496, 497, 468, 452, 282, 255, 197, 251, 338, 81, 69, 16, 263, 4, 395, 400, 298, 168, 25, 436, 277, 421, 161, 292, 54, 5, 458, 188, 403, 479, 245, 284, 216, 318, 6, 22, 195, 118, 58, 234, 356, 414, 80, 296, 29, 278, 236, 498, 191, 491, 246, 95, 325, 17, 116, 440, 410, 488, 464, 221, 377, 444, 265, 434, 397, 225, 166, 94, 442, 98, 372, 337, 300, 215, 36, 496, 462, 338, 40, 196, 250, 429, 62, 255, 455, 444, 292, 38, 256, 120, 211, 427, 414, 247, 293, 483, 204, 97, 25, 181, 434, 177, 156, 491, 371, 165, 128, 251, 383, 400, 81, 351, 295, 263, 271, 102, 86, 99, 298, 497, 348, 168, 4, 375, 325, 395, 452, 105, 450, 330, 96, 78, 188, 246, 112, 277, 174, 205, 108, 479, 498, 69, 464, 162, 410, 419, 266, 494, 421, 23, 53, 281, 314, 418, 425, 447, 440, 221, 206, 236, 80, 93, 192, 282, 101, 166, 300, 442, 337, 196, 372, 462, 215, 250, 455, 36, 496, 429, 338, 247, 38, 255, 256, 427, 165, 293, 128, 211, 292, 483, 444, 86, 156, 414, 102, 120, 383, 371, 25, 375, 251, 181, 97, 450, 497, 434, 298, 491, 266, 400, 263, 452, 108, 395, 351, 4, 168, 282, 271, 99, 348, 468, 295, 69, 188, 277, 197, 325, 246, 105, 421, 78, 112, 410, 479, 96, 162, 236, 80, 330, 221, 174, 494, 23, 419, 53, 278, 418, 464, 447, 101, 357, 161, 16, 205, 440, 54, 425, 195, 139, 5, 206, 216, 436, 22, 166, 30, 225, 94, 462, 442, 337, 98, 299, 36, 455, 215, 247, 300, 102, 211, 196, 128, 177, 168, 120, 69, 204, 282, 429, 444, 40, 479, 263, 452, 165, 86, 375, 161, 62, 496, 5, 54, 277, 483, 458, 197, 38, 81, 250, 156, 4, 497, 400, 427, 434, 330, 17, 205, 77, 289, 397, 255, 265, 377, 295, 251, 116, 132, 16, 29, 113, 293, 371, 414, 440, 281, 314, 233, 488, 271, 195, 97, 491, 454, 93, 192, 12, 403, 325, 353, 6, 15, 436, 468, 318, 206, 362, 122, 146, 356, 95, 234, 421, 99, 225, 166, 94, 442, 98, 337, 196, 462, 215, 250, 36, 338, 204, 40, 429, 62, 496, 256, 247, 38, 165, 427, 255, 128, 211, 177, 293, 102, 383, 292, 86, 483, 444, 156, 414, 120, 375, 371, 25, 251, 181, 497, 81, 263, 452, 450, 434, 97, 400, 298, 168, 266, 4, 491, 282, 69, 395, 108, 351, 197, 277, 348, 188, 271, 468, 479, 295, 246, 99, 498, 105, 410, 325, 162, 78, 96, 161, 112, 494, 421, 330, 23, 53, 418, 447, 357, 174, 54, 80, 236, 221, 419, 101, 5, 16, 278, 458, 440, 205, 139, 30, 225, 94, 98, 299, 337, 372, 204, 455, 250, 215, 247, 165, 429, 62, 256, 36, 38, 40, 128, 496, 86, 427, 338, 102, 383, 177, 293, 255, 211, 483, 156, 375, 371, 292, 450, 497, 251, 266, 414, 452, 444, 81, 25, 108, 263, 298, 282, 120, 197, 468, 4, 69, 181, 168, 434, 491, 97, 188, 351, 277, 348, 246, 421, 498, 271, 16, 410, 479, 161, 162, 80, 295, 325, 236, 494, 99, 23, 278, 54, 447, 53, 221, 105, 418, 436, 96, 101, 5, 174, 357, 112, 330, 464, 195, 78, 458, 440, 403, 139, 166, 94, 300, 98, 372, 215, 250, 462, 496, 40, 36, 455, 429, 338, 38, 255, 427, 292, 204, 293, 247, 256, 444, 211, 165, 414, 483, 156, 128, 120, 177, 25, 181, 371, 86, 97, 434, 102, 491, 450, 251, 375, 351, 298, 383, 266, 108, 395, 400, 4, 81, 497, 263, 99, 271, 348, 295, 78, 168, 112, 105, 452, 325, 246, 188, 96, 419, 236, 80, 498, 174, 221, 277, 410, 162, 278, 421, 494, 23, 53, 418, 197, 447, 468, 464, 69, 101, 357, 330, 139, 282, 425, 479, 440, 369, 127, 205, 206, 281, 146, 414, 181, 246, 292, 348, 78, 419, 369, 174, 357, 410, 447, 96, 105, 139, 278, 23, 351, 101, 236, 494, 162, 221, 395, 498, 188, 80, 418, 112, 53, 298, 94, 225, 30, 166, 196, 98, 204, 337, 442, 165, 299, 455, 247, 250, 462, 128, 372, 86, 62, 38, 429, 427, 102, 256, 215, 375, 40, 293, 177, 383, 156, 496, 450, 266, 483, 371, 255, 36, 108, 497, 211, 468, 452, 251, 282, 338, 197, 395, 292, 81, 298, 25, 400, 16, 263, 69, 4, 421, 414, 277, 436, 188, 168, 161, 80, 236, 54, 278, 216, 245, 491, 221, 498, 22, 246, 5, 403, 97, 284, 118, 444, 325, 410, 434, 351, 181, 458, 195, 191, 464, 479, 162, 318, 6, 494, 271, 23, 447, 58, 53, 225, 166, 94, 299, 442, 98, 337, 372, 300, 462, 196, 215, 36, 338, 250, 455, 40, 496, 429, 62, 256, 38, 247, 255, 204, 211, 444, 427, 292, 120, 293, 414, 483, 177, 128, 165, 383, 156, 102, 181, 86, 25, 97, 434, 371, 375, 251, 491, 263, 81, 168, 400, 4, 497, 298, 351, 452, 450, 295, 271, 348, 69, 395, 277, 99, 282, 479, 266, 188, 197, 108, 325, 246, 105, 498, 330, 78, 96, 112, 410, 162, 174, 494, 23, 53, 205, 357, 418, 447, 419, 421, 440, 101, 161, 236, 80, 221, 464, 468, 54, 94, 30, 225, 196, 165, 337, 462, 455, 102, 86, 372, 442, 177, 375, 299, 38, 62, 250, 429, 452, 427, 156, 197, 69, 211, 497, 468, 266, 215, 36, 483, 450, 16, 293, 161, 371, 338, 168, 54, 5, 263, 81, 108, 458, 277, 40, 479, 4, 251, 298, 400, 395, 188, 436, 29, 116, 17, 421, 377, 289, 496, 403, 265, 397, 498, 284, 195, 6, 77, 255, 318, 246, 357, 410, 234, 488, 132, 356, 58, 296, 23, 101, 95, 447, 53, 113, 162, 418, 494, 245, 139, 216, 440, 22, 278, 236, 80, 254, 118, 233, 300, 94, 225, 30, 166, 337, 165, 442, 455, 299, 247, 250, 128, 462, 372, 86, 38, 62, 427, 102, 429, 256, 375, 177, 156, 293, 266, 450, 215, 40, 383, 108, 371, 496, 255, 483, 211, 36, 468, 497, 282, 197, 338, 452, 251, 25, 4, 16, 292, 69, 298, 81, 395, 277, 400, 263, 168, 421, 436, 161, 54, 5, 403, 284, 491, 245, 216, 458, 22, 97, 444, 318, 479, 6, 118, 234, 325, 236, 356, 58, 80, 99, 188, 414, 296, 191, 278, 434, 29, 95, 464, 221, 195, 17, 351, 116, 120, 377, 440, 265, 166, 196, 442, 337, 372, 462, 215, 429, 247, 38, 40, 165, 496, 427, 128, 256, 255, 338, 293, 86, 177, 156, 292, 102, 211, 483, 375, 371, 266, 383, 414, 25, 108, 444, 497, 251, 181, 395, 120, 81, 468, 97, 400, 4, 491, 351, 263, 282, 434, 197, 168, 188, 69, 277, 236, 80, 348, 421, 99, 246, 498, 221, 271, 278, 105, 325, 112, 78, 410, 16, 162, 419, 96, 494, 23, 174, 295, 53, 418, 447, 101, 464, 357, 479, 436, 139, 161, 216, 54, 245, 22, 403, 330, 440, 5, 425, 195, 118, 458, 191, 442, 300, 372, 196, 215, 462, 250, 36, 40, 338, 496, 62, 429, 38, 255, 204, 256, 247, 292, 293, 211, 444, 414, 165, 128, 177, 483, 120, 156, 181, 102, 25, 86, 371, 383, 375, 97, 434, 351, 251, 491, 450, 266, 395, 4, 497, 81, 400, 263, 108, 348, 168, 105, 452, 271, 78, 99, 112, 246, 188, 96, 295, 277, 498, 282, 419, 174, 410, 69, 236, 162, 80, 221, 197, 325, 494, 23, 357, 53, 418, 278, 447, 468, 101, 479, 139, 421, 330, 369, 464, 425, 205, 440, 161, 54, 206, 127, 16, 5, 281, 215, 455, 372, 462, 36, 177, 211, 102, 491, 338, 99, 444, 400, 181, 4, 434, 351, 81, 256, 436, 271, 197, 16, 127, 425, 452, 284, 120, 295, 403, 112, 78, 419, 263, 318, 58, 478, 356, 234, 440, 277, 105, 296, 95, 6, 206, 146, 174, 195, 246, 188, 96, 498, 282, 348, 410, 383, 254, 162, 139, 488, 362, 494, 418, 330, 23, 53, 447, 129, 499, 273, 281, 101, 290, 48, 136, 104, 228, 485, 227, 260, 52, 144, 1, 473, 186, 390, 268, 63, 381, 72, 43, 406, 474, 294, 407, 122, 426, 230, 446, 225, 166, 299, 300, 442, 98, 337, 196, 372, 462, 250, 215, 40, 496, 455, 429, 36, 62, 204, 338, 247, 38, 256, 427, 128, 293, 211, 483, 86, 156, 177, 444, 292, 102, 383, 371, 120, 375, 25, 414, 251, 450, 97, 497, 266, 434, 108, 491, 400, 452, 263, 181, 4, 168, 298, 468, 271, 325, 282, 295, 99, 395, 197, 69, 421, 277, 351, 464, 479, 330, 348, 440, 188, 16, 436, 205, 161, 216, 425, 195, 54, 22, 246, 245, 105, 498, 281, 206, 403, 118, 5, 80, 146, 236, 410, 488, 127, 221, 191, 314, 98, 455, 256, 429, 338, 38, 427, 211, 177, 483, 255, 375, 293, 156, 371, 282, 414, 292, 444, 69, 197, 168, 120, 450, 298, 468, 266, 25, 108, 479, 4, 277, 161, 181, 16, 54, 97, 348, 421, 491, 5, 271, 458, 295, 325, 330, 436, 351, 101, 488, 105, 205, 80, 236, 357, 440, 464, 29, 17, 278, 96, 116, 99, 403, 221, 174, 265, 289, 216, 397, 245, 377, 233, 6, 22, 132, 77, 318, 281, 139, 454, 118, 112, 146, 356, 78, 206, 191, 93, 58, 95, 234, 362, 284, 113, 12, 296, 192, 419, 254, 30, 299, 225, 442, 372, 215, 36, 496, 337, 444, 462, 250, 429, 300, 62, 455, 292, 434, 38, 196, 491, 25, 293, 483, 295, 271, 181, 99, 177, 247, 156, 251, 168, 371, 263, 400, 205, 81, 4, 281, 479, 192, 93, 425, 353, 277, 12, 206, 464, 351, 440, 146, 383, 127, 497, 69, 478, 362, 348, 421, 452, 132, 454, 233, 105, 77, 397, 265, 289, 17, 195, 488, 48, 113, 1, 237, 102, 375, 128, 481, 499, 406, 140, 404, 323, 268, 35, 441, 157, 171, 408, 461, 377, 473, 393, 136, 7, 125, 459, 294, 30, 94, 166, 300, 442, 196, 98, 337, 462, 204, 455, 247, 256, 250, 215, 429, 36, 86, 102, 38, 338, 40, 177, 427, 496, 375, 211, 156, 293, 483, 255, 452, 282, 497, 371, 69, 263, 450, 266, 251, 81, 197, 168, 444, 120, 4, 400, 468, 108, 277, 292, 161, 479, 25, 54, 414, 16, 5, 298, 434, 395, 458, 421, 491, 97, 188, 436, 17, 271, 325, 195, 181, 403, 265, 246, 29, 295, 397, 498, 289, 440, 377, 116, 330, 99, 77, 410, 488, 132, 6, 348, 464, 318, 205, 351, 162, 356, 234, 245, 23, 337, 98, 300, 338, 196, 40, 496, 211, 250, 204, 62, 128, 444, 120, 38, 165, 255, 168, 452, 263, 81, 375, 479, 156, 400, 293, 434, 497, 197, 292, 277, 161, 295, 4, 5, 54, 97, 491, 458, 181, 271, 25, 188, 17, 440, 325, 298, 289, 77, 246, 195, 281, 397, 265, 16, 498, 132, 314, 99, 377, 116, 410, 488, 93, 29, 192, 357, 494, 351, 12, 162, 113, 53, 395, 23, 105, 468, 447, 418, 146, 421, 96, 206, 353, 101, 403, 362, 464, 174, 6, 436, 15, 318, 425, 122, 356, 95, 234, 406, 1, 94, 225, 30, 166, 196, 204, 98, 337, 462, 165, 247, 442, 455, 128, 372, 256, 86, 102, 299, 250, 429, 62, 375, 177, 38, 215, 427, 282, 452, 156, 36, 211, 497, 483, 40, 293, 371, 338, 197, 468, 69, 266, 450, 81, 496, 263, 251, 16, 108, 168, 161, 54, 400, 255, 5, 277, 458, 479, 4, 436, 395, 421, 298, 29, 116, 188, 195, 17, 403, 377, 265, 289, 284, 488, 397, 6, 318, 25, 77, 245, 216, 234, 58, 356, 132, 296, 22, 498, 440, 95, 118, 325, 246, 330, 444, 191, 410, 113, 120, 434, 225, 94, 98, 300, 196, 462, 215, 36, 338, 250, 40, 496, 62, 256, 429, 38, 247, 204, 255, 427, 211, 444, 177, 165, 120, 156, 292, 102, 483, 383, 86, 414, 375, 25, 371, 181, 97, 434, 491, 168, 251, 263, 450, 400, 81, 497, 266, 69, 277, 282, 452, 108, 351, 298, 197, 271, 479, 99, 295, 395, 348, 325, 161, 330, 468, 54, 246, 188, 78, 421, 105, 112, 5, 440, 498, 205, 96, 458, 410, 357, 162, 425, 419, 174, 464, 494, 16, 53, 17, 23, 418, 447, 281, 265, 403, 436, 397, 236, 80, 221, 225, 166, 94, 442, 98, 372, 337, 300, 215, 36, 496, 338, 462, 40, 196, 250, 429, 62, 255, 455, 444, 292, 38, 256, 120, 211, 414, 427, 247, 293, 483, 204, 181, 97, 25, 434, 177, 156, 491, 371, 251, 128, 165, 383, 351, 400, 81, 295, 263, 271, 102, 298, 99, 348, 86, 497, 4, 168, 325, 395, 375, 452, 105, 450, 330, 96, 78, 188, 246, 112, 174, 277, 498, 205, 108, 479, 419, 162, 410, 464, 69, 494, 266, 23, 53, 421, 418, 447, 281, 425, 314, 221, 236, 80, 440, 206, 101, 93, 192, 357, 94, 225, 30, 166, 196, 98, 204, 337, 442, 299, 165, 462, 455, 247, 250, 372, 128, 86, 62, 429, 38, 427, 102, 256, 215, 40, 375, 293, 177, 383, 496, 156, 450, 266, 483, 371, 36, 255, 211, 497, 108, 468, 452, 251, 338, 282, 197, 81, 292, 395, 25, 298, 400, 16, 263, 69, 4, 421, 414, 277, 436, 168, 188, 161, 54, 80, 216, 245, 236, 491, 278, 444, 5, 498, 22, 97, 221, 325, 403, 434, 246, 118, 284, 410, 195, 458, 479, 464, 181, 351, 191, 318, 271, 6, 162, 99, 58, 494, 29, 23, 225, 30, 94, 166, 299, 98, 196, 337, 372, 250, 215, 40, 462, 429, 62, 36, 255, 455, 338, 38, 204, 427, 247, 293, 165, 292, 483, 211, 256, 444, 128, 25, 371, 156, 86, 414, 120, 251, 177, 97, 450, 434, 491, 102, 375, 497, 181, 383, 400, 266, 81, 108, 271, 263, 295, 298, 99, 325, 452, 395, 351, 4, 468, 421, 464, 348, 168, 330, 105, 277, 197, 188, 216, 425, 245, 22, 440, 80, 69, 236, 246, 205, 282, 221, 206, 96, 118, 174, 146, 281, 498, 112, 195, 78, 278, 436, 127, 479, 191, 410, 300, 30, 166, 225, 196, 98, 337, 455, 165, 247, 462, 128, 256, 372, 86, 442, 102, 383, 375, 299, 177, 38, 250, 62, 427, 429, 156, 266, 211, 215, 452, 450, 197, 293, 497, 36, 468, 69, 338, 483, 371, 108, 40, 16, 168, 161, 54, 277, 263, 81, 5, 4, 458, 251, 496, 255, 479, 298, 400, 436, 395, 421, 29, 116, 403, 17, 188, 377, 284, 289, 6, 397, 318, 265, 234, 77, 195, 356, 58, 296, 498, 95, 132, 25, 488, 245, 246, 216, 22, 357, 440, 410, 113, 120, 118, 191, 23, 325, 101, 236, 30, 166, 94, 299, 442, 98, 300, 337, 372, 196, 215, 250, 496, 36, 338, 429, 255, 462, 38, 455, 427, 293, 444, 292, 211, 120, 247, 25, 483, 204, 97, 414, 256, 156, 165, 434, 371, 491, 128, 177, 251, 181, 450, 400, 271, 99, 86, 295, 108, 266, 81, 325, 375, 497, 263, 4, 351, 102, 383, 298, 464, 421, 395, 330, 168, 452, 425, 277, 127, 348, 468, 205, 206, 281, 440, 314, 105, 146, 78, 216, 22, 192, 93, 112, 245, 12, 479, 478, 96, 118, 419, 174, 69, 353, 197, 195, 191, 236, 246, 80, 30, 299, 442, 300, 337, 196, 372, 215, 462, 455, 40, 496, 36, 62, 429, 338, 38, 204, 255, 247, 293, 256, 292, 165, 211, 128, 444, 483, 156, 177, 414, 120, 25, 86, 371, 102, 375, 450, 181, 97, 251, 266, 491, 383, 434, 108, 497, 298, 351, 400, 81, 4, 263, 395, 99, 271, 452, 168, 295, 468, 325, 282, 277, 348, 197, 105, 69, 78, 421, 112, 188, 246, 96, 236, 80, 498, 419, 221, 174, 330, 464, 479, 410, 278, 162, 494, 425, 23, 53, 418, 440, 447, 357, 16, 205, 101, 127, 206, 161, 22, 300, 94, 225, 166, 98, 337, 165, 442, 247, 128, 250, 299, 372, 462, 38, 86, 62, 102, 427, 375, 256, 177, 156, 450, 293, 429, 108, 383, 215, 40, 211, 371, 483, 36, 468, 255, 282, 496, 197, 497, 4, 338, 452, 16, 69, 298, 251, 25, 292, 277, 395, 168, 81, 263, 436, 400, 161, 54, 421, 5, 403, 458, 284, 318, 6, 479, 234, 491, 236, 80, 245, 356, 58, 278, 216, 22, 296, 444, 188, 351, 221, 95, 97, 17, 377, 118, 99, 139, 29, 414, 246, 191, 498, 265, 440, 397, 181, 325, 116, 410, 166, 299, 98, 215, 462, 36, 300, 40, 496, 196, 250, 455, 256, 62, 444, 255, 120, 211, 38, 247, 414, 292, 427, 483, 383, 293, 177, 181, 204, 434, 97, 128, 156, 25, 102, 491, 263, 168, 165, 251, 81, 371, 400, 295, 271, 351, 4, 452, 348, 86, 375, 69, 479, 497, 99, 298, 277, 330, 325, 205, 105, 188, 282, 246, 395, 96, 78, 498, 174, 410, 112, 162, 197, 281, 494, 314, 440, 53, 23, 357, 418, 447, 419, 161, 192, 93, 464, 101, 12, 206, 421, 425, 450, 54, 5, 369, 146, 353, 458, 17, 215, 196, 40, 250, 496, 429, 62, 247, 444, 255, 427, 204, 177, 292, 383, 414, 293, 483, 128, 102, 165, 181, 434, 97, 168, 25, 375, 263, 86, 491, 371, 251, 81, 4, 400, 69, 452, 351, 282, 277, 497, 479, 295, 271, 348, 99, 197, 330, 450, 395, 188, 325, 105, 246, 161, 205, 78, 96, 498, 357, 54, 112, 266, 410, 108, 5, 162, 174, 494, 440, 23, 53, 458, 418, 447, 419, 281, 101, 314, 425, 17, 464, 206, 192, 93, 397, 77, 369, 265, 468, 289, 132, 139, 12, 146, 377, 221, 236, 195, 80, 225, 30, 166, 337, 462, 383, 197, 442, 497, 372, 483, 4, 436, 452, 69, 298, 395, 211, 277, 421, 161, 54, 299, 403, 245, 251, 5, 318, 234, 58, 216, 6, 22, 458, 296, 255, 118, 356, 191, 25, 95, 168, 80, 236, 278, 29, 81, 400, 116, 40, 377, 221, 17, 263, 265, 397, 139, 289, 195, 254, 77, 132, 479, 440, 188, 488, 464, 127, 498, 113, 273, 410, 357, 246, 215, 425, 325, 260, 186, 445, 292, 485, 290, 474, 228, 294, 104, 145, 129, 144, 72, 390, 136, 1, 0, 478, 499, 305, 39, 155, 337, 40, 338, 62, 120, 97, 434, 256, 156, 204, 99, 165, 128, 450, 325, 263, 497, 108, 298, 4, 266, 86, 464, 395, 375, 102, 425, 205, 78, 421, 112, 383, 168, 206, 281, 452, 314, 127, 277, 419, 440, 192, 93, 246, 12, 221, 478, 80, 479, 236, 353, 188, 498, 162, 216, 22, 468, 410, 494, 278, 245, 195, 53, 362, 418, 23, 69, 447, 118, 191, 122, 369, 15, 454, 488, 233, 197, 101, 403, 132, 48, 1, 357, 436, 282, 499, 237, 356, 318, 473, 17, 406, 461, 268, 171, 441, 136, 265, 323, 481, 94, 225, 30, 166, 196, 442, 299, 337, 204, 250, 455, 372, 462, 165, 247, 62, 429, 38, 128, 215, 427, 86, 40, 496, 256, 293, 36, 102, 255, 177, 156, 375, 338, 483, 211, 383, 450, 371, 266, 108, 497, 292, 468, 251, 25, 452, 282, 81, 197, 298, 400, 444, 263, 395, 414, 4, 69, 16, 97, 491, 120, 421, 277, 168, 434, 181, 325, 161, 271, 351, 99, 436, 188, 54, 216, 245, 479, 236, 80, 22, 5, 464, 498, 278, 221, 295, 246, 403, 118, 458, 284, 348, 195, 410, 191, 318, 330, 105, 440, 6, 225, 166, 299, 98, 372, 196, 462, 215, 36, 455, 250, 40, 496, 429, 256, 62, 247, 204, 38, 255, 211, 427, 444, 165, 292, 128, 293, 120, 414, 383, 177, 483, 102, 156, 86, 181, 375, 371, 25, 434, 97, 251, 263, 81, 168, 491, 497, 452, 298, 400, 4, 351, 395, 69, 450, 348, 282, 271, 295, 188, 277, 266, 479, 197, 246, 99, 108, 498, 105, 325, 410, 78, 96, 162, 112, 494, 330, 23, 53, 174, 418, 357, 447, 101, 421, 419, 161, 468, 236, 80, 205, 221, 54, 440, 278, 5, 464, 139, 458, 281, 30, 94, 166, 299, 442, 300, 98, 337, 196, 372, 250, 215, 462, 40, 496, 455, 36, 429, 338, 38, 255, 204, 427, 247, 293, 165, 256, 211, 128, 444, 292, 156, 483, 177, 25, 120, 86, 371, 102, 450, 375, 97, 414, 251, 383, 266, 491, 434, 108, 181, 497, 400, 4, 81, 263, 99, 271, 452, 295, 298, 168, 325, 351, 468, 277, 395, 421, 197, 282, 69, 464, 330, 479, 425, 348, 440, 127, 436, 22, 216, 206, 245, 78, 205, 16, 105, 112, 188, 281, 146, 246, 161, 403, 195, 236, 80, 54, 118, 498, 221, 225, 166, 94, 299, 442, 98, 337, 372, 300, 462, 196, 215, 36, 338, 250, 455, 40, 496, 429, 256, 62, 247, 38, 255, 204, 211, 444, 427, 292, 120, 293, 414, 483, 128, 165, 177, 383, 156, 102, 86, 181, 25, 434, 97, 371, 375, 251, 263, 491, 81, 168, 400, 497, 452, 4, 298, 351, 69, 295, 271, 450, 348, 395, 282, 277, 99, 479, 188, 266, 197, 325, 108, 246, 105, 330, 498, 78, 96, 410, 112, 162, 494, 174, 205, 23, 53, 357, 418, 447, 419, 421, 161, 440, 101, 54, 468, 236, 464, 80, 221, 36, 38, 455, 427, 414, 97, 483, 256, 247, 156, 371, 295, 251, 351, 400, 177, 325, 81, 4, 204, 330, 450, 348, 298, 464, 105, 108, 497, 425, 128, 205, 165, 78, 314, 96, 168, 281, 112, 395, 174, 127, 192, 93, 421, 266, 419, 146, 12, 440, 277, 353, 375, 478, 479, 246, 383, 452, 221, 236, 162, 80, 498, 102, 86, 188, 22, 494, 216, 410, 122, 369, 15, 23, 362, 53, 278, 245, 418, 195, 447, 118, 454, 191, 488, 233, 69, 101, 132, 48, 237, 403, 499, 1, 357, 473, 481, 406, 268, 468, 140, 372, 98, 337, 255, 462, 250, 429, 211, 300, 181, 491, 25, 455, 483, 38, 196, 295, 293, 427, 271, 99, 251, 325, 330, 400, 263, 351, 81, 371, 281, 156, 348, 247, 192, 93, 12, 177, 4, 425, 464, 206, 105, 479, 440, 146, 96, 78, 127, 277, 174, 112, 383, 478, 122, 15, 419, 298, 497, 233, 246, 454, 421, 369, 362, 452, 69, 162, 132, 195, 498, 494, 488, 188, 410, 53, 23, 48, 237, 418, 77, 499, 397, 395, 140, 441, 7, 481, 447, 406, 1, 113, 473, 323, 265, 171, 17, 461, 404, 33, 393, 30, 225, 94, 299, 442, 98, 372, 337, 300, 462, 196, 215, 338, 36, 455, 250, 40, 256, 496, 429, 62, 247, 204, 38, 255, 211, 444, 427, 120, 177, 383, 128, 293, 483, 165, 102, 292, 414, 156, 86, 375, 371, 434, 25, 181, 263, 97, 251, 168, 81, 491, 452, 400, 497, 69, 282, 4, 295, 277, 271, 479, 298, 351, 197, 348, 450, 99, 330, 395, 325, 266, 188, 108, 161, 246, 205, 105, 498, 54, 5, 96, 468, 410, 458, 162, 440, 78, 421, 494, 357, 112, 174, 23, 53, 447, 418, 281, 464, 314, 16, 98, 128, 247, 62, 299, 383, 293, 429, 282, 468, 371, 215, 69, 36, 497, 16, 40, 452, 277, 338, 161, 54, 255, 5, 458, 263, 251, 496, 479, 298, 421, 81, 400, 284, 395, 25, 377, 17, 397, 265, 289, 29, 77, 245, 116, 132, 22, 216, 292, 118, 191, 195, 113, 491, 488, 357, 99, 278, 120, 188, 80, 444, 236, 325, 464, 97, 221, 425, 254, 127, 246, 410, 498, 273, 351, 434, 233, 478, 485, 447, 445, 499, 474, 23, 101, 78, 112, 390, 260, 294, 104, 53, 1, 72, 418, 136, 407, 270, 162, 146, 442, 372, 196, 338, 62, 165, 255, 427, 128, 211, 383, 177, 293, 86, 444, 292, 414, 156, 120, 371, 375, 251, 81, 25, 263, 400, 434, 181, 97, 282, 450, 298, 168, 491, 69, 395, 266, 4, 197, 108, 271, 351, 295, 348, 468, 188, 277, 479, 325, 99, 246, 330, 498, 105, 421, 410, 161, 162, 494, 96, 23, 54, 53, 205, 16, 418, 447, 174, 5, 464, 112, 78, 101, 80, 440, 236, 458, 357, 195, 221, 278, 419, 281, 436, 488, 206, 146, 17, 403, 425, 139, 314, 93, 216, 397, 29, 265, 192, 22, 289, 166, 30, 225, 94, 372, 442, 299, 337, 300, 98, 256, 338, 215, 36, 455, 196, 383, 247, 429, 204, 102, 211, 40, 128, 250, 496, 165, 177, 62, 86, 483, 444, 452, 38, 120, 69, 168, 263, 282, 81, 375, 427, 255, 479, 497, 400, 251, 156, 414, 197, 371, 161, 293, 434, 54, 5, 277, 458, 4, 330, 295, 205, 271, 491, 17, 188, 16, 97, 195, 440, 289, 77, 397, 292, 348, 265, 488, 116, 377, 29, 132, 468, 233, 25, 281, 325, 454, 181, 113, 93, 192, 12, 314, 246, 498, 403, 298, 421, 410, 436, 225, 94, 98, 337, 372, 36, 250, 455, 338, 429, 40, 496, 62, 247, 204, 38, 211, 255, 165, 427, 128, 383, 292, 444, 483, 414, 177, 293, 102, 86, 120, 156, 371, 181, 375, 251, 81, 263, 25, 452, 434, 497, 400, 97, 298, 168, 491, 395, 348, 351, 69, 4, 188, 282, 450, 295, 271, 246, 197, 498, 479, 105, 410, 277, 266, 162, 494, 96, 108, 99, 23, 53, 447, 418, 325, 330, 174, 78, 357, 112, 101, 468, 80, 236, 161, 205, 421, 221, 419, 278, 54, 440, 139, 5, 464, 195, 458, 369, 281, 16, 30, 299, 300, 98, 337, 196, 462, 215, 250, 455, 36, 338, 40, 496, 62, 256, 247, 204, 38, 255, 211, 165, 293, 444, 483, 292, 177, 156, 383, 120, 102, 86, 414, 25, 375, 251, 97, 434, 181, 497, 491, 81, 263, 400, 450, 452, 168, 4, 266, 298, 108, 271, 295, 69, 351, 282, 395, 277, 99, 197, 348, 325, 479, 468, 330, 188, 421, 246, 105, 498, 464, 205, 96, 410, 161, 440, 78, 162, 112, 174, 494, 54, 53, 23, 418, 80, 447, 236, 221, 5, 16, 425, 281, 195, 419, 357, 206, 436, 278, 458, 30, 166, 225, 94, 442, 372, 215, 98, 36, 337, 496, 40, 462, 444, 255, 250, 429, 120, 300, 62, 196, 211, 455, 292, 256, 414, 97, 434, 38, 427, 483, 293, 25, 491, 181, 295, 247, 271, 251, 99, 263, 177, 400, 156, 371, 81, 330, 325, 168, 383, 205, 314, 351, 4, 479, 281, 192, 93, 277, 12, 348, 464, 425, 353, 206, 497, 440, 452, 146, 128, 69, 105, 127, 102, 478, 15, 421, 122, 96, 298, 78, 174, 233, 454, 362, 112, 195, 375, 132, 204, 488, 77, 17, 397, 265, 419, 246, 289, 113, 282, 299, 442, 98, 337, 300, 372, 462, 196, 215, 36, 338, 455, 250, 40, 496, 429, 256, 62, 247, 204, 38, 255, 211, 427, 292, 444, 414, 120, 128, 293, 165, 177, 483, 383, 102, 156, 181, 86, 371, 434, 25, 375, 97, 251, 263, 81, 298, 491, 168, 351, 452, 400, 497, 348, 395, 4, 69, 188, 282, 271, 295, 246, 450, 105, 277, 498, 479, 410, 96, 99, 78, 197, 162, 112, 494, 174, 23, 266, 53, 357, 418, 447, 325, 108, 330, 419, 101, 236, 80, 221, 205, 278, 161, 139, 369, 421, 468, 54, 440, 5, 196, 442, 211, 38, 156, 263, 62, 36, 427, 4, 429, 81, 266, 483, 17, 299, 289, 250, 377, 77, 397, 265, 215, 371, 450, 436, 188, 108, 293, 132, 400, 403, 113, 6, 357, 318, 234, 195, 251, 284, 488, 356, 296, 58, 298, 95, 498, 246, 421, 410, 395, 440, 233, 23, 101, 447, 53, 418, 139, 454, 494, 254, 162, 330, 205, 445, 273, 120, 301, 252, 474, 294, 362, 380, 406, 142, 485, 1, 145, 270, 157, 409, 260, 40, 290, 35, 407, 72, 144, 33, 390, 0, 481, 136, 228, 441, 230, 7, 361, 323, 337, 98, 372, 299, 247, 455, 128, 102, 250, 36, 62, 338, 177, 38, 375, 40, 483, 427, 496, 282, 497, 156, 69, 371, 197, 293, 168, 251, 255, 400, 468, 161, 414, 395, 479, 298, 450, 266, 54, 5, 16, 277, 458, 4, 444, 120, 498, 108, 292, 246, 410, 348, 23, 421, 494, 162, 53, 447, 436, 418, 434, 195, 357, 29, 116, 17, 25, 330, 403, 289, 265, 397, 377, 440, 181, 325, 77, 6, 205, 80, 139, 278, 295, 236, 271, 132, 318, 105, 491, 221, 96, 356, 233, 351, 234, 97, 58, 95, 464, 113, 255, 300, 62, 211, 292, 196, 455, 434, 38, 491, 427, 25, 293, 181, 271, 247, 251, 263, 99, 400, 330, 81, 177, 371, 205, 383, 314, 351, 4, 348, 353, 206, 425, 277, 497, 146, 452, 69, 105, 127, 96, 122, 15, 421, 478, 128, 102, 233, 78, 454, 174, 298, 362, 488, 132, 195, 112, 246, 77, 397, 17, 265, 419, 188, 113, 289, 237, 48, 162, 499, 406, 441, 1, 7, 498, 494, 481, 323, 161, 369, 473, 140, 33, 377, 157, 171, 461, 408, 409, 302, 410, 395, 268, 136, 380, 404, 185, 459, 35, 311, 300, 94, 225, 30, 166, 98, 204, 337, 442, 165, 250, 455, 299, 38, 62, 128, 247, 86, 427, 372, 462, 266, 293, 450, 156, 429, 375, 102, 177, 108, 40, 215, 256, 255, 496, 371, 483, 211, 468, 36, 25, 497, 383, 4, 197, 292, 251, 282, 16, 338, 452, 395, 298, 400, 421, 436, 81, 277, 491, 69, 263, 245, 403, 97, 216, 22, 168, 284, 99, 118, 318, 444, 6, 58, 234, 161, 191, 54, 356, 296, 325, 464, 80, 236, 95, 434, 278, 351, 221, 440, 181, 5, 271, 458, 425, 414, 127, 195, 17, 377, 225, 166, 94, 299, 442, 98, 337, 372, 300, 462, 196, 215, 36, 338, 250, 455, 40, 496, 429, 256, 62, 38, 247, 255, 204, 211, 427, 444, 120, 292, 293, 483, 165, 128, 177, 414, 383, 156, 102, 86, 25, 181, 97, 434, 371, 375, 251, 263, 491, 81, 168, 400, 497, 4, 452, 298, 351, 450, 295, 69, 271, 277, 282, 348, 99, 395, 479, 266, 108, 197, 325, 188, 330, 246, 105, 498, 78, 96, 205, 410, 112, 162, 421, 494, 174, 440, 161, 23, 53, 418, 447, 357, 468, 464, 419, 54, 101, 281, 5, 425, 30, 166, 442, 98, 300, 337, 372, 215, 496, 40, 250, 196, 36, 338, 462, 429, 62, 455, 292, 38, 444, 427, 293, 120, 414, 211, 247, 483, 256, 25, 97, 204, 181, 434, 491, 156, 371, 251, 177, 165, 128, 351, 271, 295, 400, 99, 81, 298, 263, 450, 497, 325, 86, 383, 348, 102, 395, 4, 375, 108, 105, 330, 266, 168, 452, 78, 464, 96, 112, 174, 421, 246, 205, 419, 277, 188, 425, 498, 221, 314, 162, 236, 80, 281, 206, 410, 494, 479, 440, 127, 146, 192, 93, 23, 53, 418, 278, 12, 447, 69, 94, 225, 30, 166, 300, 299, 196, 442, 337, 372, 250, 455, 462, 204, 215, 62, 40, 429, 496, 38, 247, 36, 338, 427, 255, 165, 256, 293, 128, 177, 211, 156, 86, 483, 102, 375, 371, 292, 444, 450, 25, 266, 383, 120, 108, 251, 497, 97, 414, 491, 468, 282, 434, 81, 400, 4, 452, 263, 181, 197, 168, 277, 298, 99, 271, 69, 325, 295, 421, 16, 395, 351, 479, 464, 330, 161, 54, 216, 22, 425, 5, 245, 436, 440, 458, 403, 206, 127, 118, 348, 195, 146, 105, 205, 191, 318, 284, 188, 17, 29, 30, 225, 94, 442, 98, 300, 337, 372, 215, 196, 250, 496, 462, 40, 36, 338, 429, 455, 62, 38, 427, 444, 256, 293, 211, 204, 292, 483, 120, 414, 25, 165, 177, 128, 156, 97, 371, 434, 251, 491, 181, 383, 86, 102, 400, 81, 497, 375, 271, 295, 263, 450, 99, 4, 325, 452, 108, 266, 351, 168, 298, 330, 277, 395, 69, 348, 421, 464, 282, 479, 468, 205, 105, 425, 197, 440, 281, 206, 314, 146, 96, 127, 93, 192, 188, 246, 78, 174, 12, 112, 195, 498, 216, 22, 353, 478, 161, 245, 162, 410, 30, 225, 299, 442, 300, 337, 372, 196, 462, 215, 455, 250, 36, 338, 40, 429, 496, 204, 62, 256, 247, 38, 255, 427, 211, 165, 128, 293, 177, 292, 444, 483, 102, 383, 156, 120, 86, 414, 371, 375, 25, 181, 251, 97, 497, 434, 81, 263, 450, 452, 491, 400, 298, 282, 168, 266, 4, 351, 69, 108, 395, 277, 271, 197, 295, 348, 99, 468, 479, 188, 325, 105, 246, 498, 330, 421, 161, 78, 96, 410, 112, 162, 174, 494, 54, 23, 53, 236, 357, 16, 418, 5, 80, 447, 419, 221, 205, 464, 101, 278, 30, 225, 94, 299, 442, 98, 372, 337, 300, 462, 196, 215, 338, 36, 455, 250, 40, 496, 256, 429, 62, 247, 204, 38, 255, 211, 427, 444, 177, 120, 128, 383, 293, 165, 483, 102, 292, 414, 156, 86, 371, 375, 25, 434, 97, 251, 263, 181, 81, 168, 491, 400, 452, 497, 4, 69, 282, 295, 271, 277, 479, 298, 450, 351, 197, 99, 348, 266, 330, 325, 108, 395, 161, 188, 205, 468, 54, 105, 246, 5, 498, 421, 440, 458, 96, 410, 78, 162, 464, 281, 112, 494, 174, 357, 23, 53, 16, 314, 447, 418, 94, 98, 337, 442, 372, 462, 250, 165, 247, 62, 128, 256, 38, 215, 429, 86, 427, 102, 36, 40, 177, 496, 338, 293, 375, 383, 156, 211, 255, 483, 450, 266, 371, 292, 497, 108, 282, 452, 298, 414, 251, 395, 468, 81, 4, 25, 444, 69, 263, 168, 400, 181, 120, 188, 277, 351, 16, 246, 498, 410, 348, 491, 434, 236, 80, 161, 278, 97, 221, 421, 162, 54, 357, 23, 479, 494, 447, 53, 418, 112, 5, 139, 101, 78, 105, 436, 96, 458, 419, 174, 99, 271, 403, 325, 195, 318, 6, 440, 17, 216, 429, 462, 211, 483, 414, 371, 247, 271, 251, 204, 295, 325, 165, 256, 400, 128, 375, 298, 464, 421, 263, 497, 105, 395, 81, 206, 330, 348, 440, 86, 146, 168, 22, 96, 174, 281, 314, 216, 221, 236, 245, 80, 192, 205, 468, 93, 102, 353, 12, 118, 191, 278, 403, 436, 122, 15, 356, 246, 318, 95, 479, 362, 234, 369, 132, 6, 296, 58, 162, 197, 195, 499, 48, 1, 498, 237, 473, 268, 265, 481, 284, 410, 397, 17, 228, 227, 140, 407, 139, 136, 33, 129, 231, 104, 494, 171, 380, 276, 35, 273]

        preds2 = exp.recommend(users = [1, 2], exclude_known=False)
        assert preds2['user'].unique().tolist() == [1,2]

        preds = exp.recommend(exclude_known=True)
        joined_preds = preds.join(
            data.set_index(['user', 'item']),
            on=['user','item'], how='inner'
        )
        assert len(joined_preds) == 0