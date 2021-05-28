#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# --------------------------------------------------------------------------- #
__author__ = "Juan Luis Padilla Salomé"
__copyright__ = "Copyright 2021"
__credits__ = ["University of Cantabria", "Pablo Abad", "Pablo Prieto"]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Juan Luis Padilla Salomé"
__email__ = "juan-luis.padilla@alumnos.unican.es"
__status__ = "Production"
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
if __name__ == "__main__":
    """
    TODO
    """

    # standard library
    import numpy as np
    import os

    # Forces the program to execute on CPU
    os.environ['CUDA_VISIBLE_DEVICES'] = '0'

    # Just disables the warning, doesn't take advantage of AVX/FMA to run faster
    os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

    # 3rd party packages
    # import tensorflow as tf
    from tensorflow import keras

    # ---------------- Setting up the inter and intra threads --------------- #
    # inter = tf.config.threading.get_inter_op_parallelism_threads()
    # intra = tf.config.threading.get_intra_op_parallelism_threads()

    # inter = 1
    # intra = 1

    # tf.config.threading.set_inter_op_parallelism_threads(inter)
    # tf.config.threading.set_intra_op_parallelism_threads(intra)

    # print("inter_op_parallelism_threads = {}\nintra_op_parallelism_threads = "
    #       "{}".format(tf.config.threading.get_inter_op_parallelism_threads(),
    #                   tf.config.threading.get_intra_op_parallelism_threads()))
    # ----------------------------------------------------------------------- #

    # ------------------- Load the dataset and define it -------------------- #
    # Fetch and load common datasets, including Fashion MNIST
    # It has the exact same format as MNIST (70.000 grayscale image of 28 x
    # 28 pixels each, with 10 classes).
    fashion_mnist = keras.datasets.fashion_mnist

    # Images represented as a 28 x 28 array
    # Pixel intensities are represented as integers (from 0 to 255)

    # The dataset is already split inot a training set and a test set
    (X_train_full, Y_train_full), (X_test, Y_test) = fashion_mnist.load_data()

    # We'll create a validation set which will contain 5,000 images, and
    # the test set will contain 10,000 images. So, the others 55.000 images
    # will be used as trainning set
    X_valid, X_train = X_train_full[:5000] / 255., X_train_full[5000:] / 255.
    Y_valid, Y_train = Y_train_full[:5000], Y_train_full[5000:]
    X_test = X_test / 255.

    # List of class names
    class_names = ["T-shirt/top", "Trouser", "Pullover", "Dress", "Coat",
                   "Sandal", "Shirt", "Sneaker", "Bag", "Ankle boot"]
    # ----------------------------------------------------------------------- #

    # ------------- Creating the model using the Sequential API ------------- #
    # 1. Creates a Sequential model. Layers connected sequentially
    model = keras.models.Sequential()

    # 2. Adds a flatten layer whose role is to convert each input image
    # into a 1D array.
    model.add(keras.layers.Flatten(input_shape=[28, 28]))

    # 3. Adds a dense hidden layer with 300 neurons using the ReLU
    # activation function. Each one manages its own weight matrix.
    model.add(keras.layers.Dense(300, activation="relu"))

    # 4. Adds a second dense hidden layer with 100 neurons using the ReLU
    # activation function.
    model.add(keras.layers.Dense(100, activation="relu"))

    # 5. Finaly, add a dense output layer with 10 neurons (one per class),
    # using the softmax activation function (beacuse the classes are
    # exclusive).
    model.add(keras.layers.Dense(10, activation="softmax"))

    # Compiling the model
    model.compile(loss="sparse_categorical_crossentropy",
                  optimizer="sgd",
                  metrics=["accuracy"])
    # ----------------------------------------------------------------------- #

    # ----------------------------------------------------------------------- #
    # ! TRAIN
    # ----------------------------------------------------------------------- #
    history = model.fit(x=X_train,
                        y=Y_train,
                        batch_size=None,
                        epochs=1,
                        verbose=1,
                        callbacks=None,
                        validation_split=0.,
                        validation_data=None,
                        shuffle=True,
                        class_weight=None,
                        sample_weight=None,
                        initial_epoch=0,
                        steps_per_epoch=None,
                        validation_steps=None,
                        validation_batch_size=None,
                        validation_freq=1,
                        max_queue_size=10,
                        workers=1,
                        use_multiprocessing=False)
    # ----------------------------------------------------------------------- #
    # ! END TRAIN
    # ----------------------------------------------------------------------- #

    # ----------------------------------------------------------------------- #
    # ! TEST
    # ----------------------------------------------------------------------- #
    model.evaluate(x=X_test,
                   y=Y_test,
                   batch_size=None,
                   verbose=1,
                   sample_weight=None,
                   steps=None,
                   callbacks=None,
                   max_queue_size=10,
                   workers=1,
                   use_multiprocessing=False,
                   return_dict=False)
    # ----------------------------------------------------------------------- #
    # ! END TEST
    # ----------------------------------------------------------------------- #

    # Get the last n items of the test dataset
    n_items = 15
    X_new = X_test[:n_items]

    # ----------------------------------------------------------------------- #
    # ! PREDICT
    # ----------------------------------------------------------------------- #
    y_proba = model.predict(x=X_new,
                            batch_size=None,
                            verbose=0,
                            steps=None,
                            callbacks=None,
                            max_queue_size=10,
                            workers=1,
                            use_multiprocessing=False)
    # ----------------------------------------------------------------------- #
    # ! END PREDICT
    # ----------------------------------------------------------------------- #

    # Check if the prediction is correct:
    # From the list, get the option which has most probab.
    y_pred = np.argmax(y_proba, axis=-1)
    # We can get the correspondences between class name and num
    pred = np.array(class_names)[y_pred]

    # And check if it its correct
    y_new = Y_test[:n_items]
    sol = np.array(class_names)[y_new]

    # print("Clothes predicted: {}\nClothes tested: {}".format(pred, sol))
