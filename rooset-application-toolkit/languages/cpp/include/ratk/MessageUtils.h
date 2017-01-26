#pragma once
#include <functional>
#include <type_traits>
#include <typeinfo>
#include "exceptions/CommandEvaluationException.h"


using namespace std;

namespace rooset {
  class MessageUtils
  {
  private:
    template<class> struct sfinae_true : std::true_type {};

    template<class T, class A0>
    static auto test_has_handle(int)
      ->sfinae_true<decltype(std::declval<T>().handleEvent(std::declval<A0>()))>;

    template<class, class A0>
    static auto test_has_handle(long)->std::false_type;

    template<class T, class Arg>
    struct hasHandleEvent : decltype(test_has_handle<T, Arg>(0)){};

  public:
    template <typename Aggregate, typename Event>
    static void handleEvent(
        typename enable_if<hasHandleEvent<Aggregate, Event>::value, Aggregate>::type& a,
        const Event& e,
        std::function<void()> onMissingMethod)
    {
      a.handleEvent(e);
    }

    template <typename Aggregate, typename Event>
    static void handleEvent(
        typename enable_if<!hasHandleEvent<Aggregate, Event>::value, Aggregate>::type& a,
        const Event& e,
        std::function<void()> onMissingMethod)
    {
      onMissingMethod();
    }

    template<typename Aggregate, typename Event>
    static unique_ptr<Aggregate> instantiateAggregate(
      const typename enable_if<is_constructible<Aggregate, Event>::value, Event>::type& e,
      std::function<void()> onMissingMethod)
    {
      return make_unique<Aggregate>(e);
    }

    template<typename Aggregate, typename Event>
    static unique_ptr<Aggregate> instantiateAggregate(
      const typename enable_if<!is_constructible<Aggregate, Event>::value, Event>::type& e,
      std::function<void()> onMissingMethod)
    {
      onMissingMethod();
      return nullptr;
    }

    template<typename Aggregate, typename Event>
    static void applyEvent(
        unique_ptr<Aggregate>& a,
        const Event& e,
        std::function<void()> onMissingMethod)
    {
      if (a == nullptr) {
        if (hasHandleEvent<Aggregate, Event>::value) {
          throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION, 
              "A non-construcotr event for this aggregate has been called before the constructor event");
        }
        a = instantiateAggregate<Aggregate, Event>(e, onMissingMethod);
        return;
      }
      if (is_constructible<Aggregate, Event>::value) {
        throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
          "A constructor event has been called after another constructor event");
      }
      handleEvent<Aggregate, Event>(*a, e, onMissingMethod);
    }
  };

}
