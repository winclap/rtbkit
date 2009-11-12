/* auto_encoder_trainer.h                                          -*- C++ -*-
   Jeremy Barnes, 11 November 2009
   Copyright (c) 2009 Jeremy Barnes.  All rights reserved.

   Trainer for an auto-encoder (denoising or not).
*/

#ifndef __jml__neural__auto_encoder_trainer_h__
#define __jml__neural__auto_encoder_trainer_h__


#include "auto_encoder.h"
#include "auto_encoder_stack.h"
#include "stats/distribution.h"
#include <vector>


namespace ML {


struct Configuration;


/*****************************************************************************/
/* AUTO_ENCODER_TRAINER                                                      */
/*****************************************************************************/

/** Class that can train an auto-encoder via backpropagation. */

struct Auto_Encoder_Trainer {

    Auto_Encoder_Trainer();

    void defaults();

    void configure(const std::string & name, const Configuration & config);

    float prob_cleared;
    int minibatch_size;
    float learning_rate;
    int verbosity;
    float sample_proportion;
    bool randomize_order;
    int niter;
    int test_every;


    /** Add noise to the distribution, according to the noise parameters that
        have been set above. */
    template<typename Float>
    distribution<Float>
    add_noise(const distribution<Float> & inputs,
              Thread_Context & context) const;

    /** Train on a single example, updating the parameters. */
    std::pair<double, double>
    train_example(const Auto_Encoder & encoder,
                  const distribution<float> & example,
                  Parameters & updates,
                  Thread_Context & context) const;

    /** Trains a single iteration on the given data with the selected
        parameters.  Returns a moving estimate of the RMSE on the
        training set. */
    std::pair<double, double>
    train_iter(Auto_Encoder & encoder,
               const std::vector<distribution<float> > & data,
               Thread_Context & thread_context);

    void
    train(Auto_Encoder & encoder,
          const std::vector<distribution<float> > & training_data,
          const std::vector<distribution<float> > & testing_data,
          Thread_Context & thread_context);
    
    /** Trains an auto-encoder stack in a greedy manner by training one layer
        at a time. */
    void train_stack(Auto_Encoder_Stack & stack,
                     const std::vector<distribution<float> > & training_data,
                     const std::vector<distribution<float> > & testing_data,
                     Thread_Context & thread_context);

    
    std::pair<double, double>
    test(const Auto_Encoder & encoder,
         const std::vector<distribution<float> > & data,
         Thread_Context & context);

    /** Tests on the given dataset, returning the exact and noisy RMSE.  If
        data_out is non-empty, then it will also fill it in with the
        hidden representations for each of the inputs (with no noise added).
        This information can be used to train the next layer. */
    std::pair<double, double>
    test_and_update(const Auto_Encoder & encoder,
                    const std::vector<distribution<float> > & data_in,
                    std::vector<distribution<float> > & data_out,
                    Thread_Context & thread_context) const;

    /** Tests on the given dataset, returning the exact and noisy RMSE. */
    std::pair<double, double>
    test(const Auto_Encoder & encoder,
         const std::vector<distribution<float> > & data,
         Thread_Context & thread_context) const
    {
        std::vector<distribution<float> > dummy;
        return test_and_update(encoder, data, dummy, thread_context);
    }
};
    
} // namespace ML

#endif /* __jml__neural__auto_encoder_trainer_h__ */

