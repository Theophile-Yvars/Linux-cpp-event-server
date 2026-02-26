# Linux-cpp-event-server

## Architecture

```
                           ┌─────────────────────┐
                           │      Thread I/O     │
                           │  (collect events)   │
                           └──────────┬──────────┘
                                      │ push(event)
                                      ▼
                           ┌─────────────────────┐
                           │  ThreadSafeQueue    │
                           │  (shared buffer)    │
                           └──────────┬──────────┘
                                      │ pop()
        ┌─────────────────────────────┼─────────────────────────────┐
        ▼                             ▼                             ▼
┌────────────────┐          ┌────────────────┐          ┌────────────────┐
│    Worker 1    │          │    Worker 2    │   ...    │    Worker N    │
│ (thread loop)  │          │ (thread loop)  │          │ (thread loop)  │
└────────┬───────┘          └────────┬───────┘          └────────┬───────┘
         │                            │                            │
         ▼                            ▼                            ▼
   ┌───────────────┐           ┌───────────────┐           ┌───────────────┐
   │  RuleEngine   │           │  RuleEngine   │           │  RuleEngine   │
   │  evaluate()   │           │  evaluate()   │           │  evaluate()   │
   └──────┬────────┘           └──────┬────────┘           └──────┬────────┘
          │                             │                             │
          ▼                             ▼                             ▼
   ┌───────────────┐           ┌───────────────┐           ┌───────────────┐
   │    Action     │           │    Action     │           │    Action     │
   │ (log/alert)   │           │ (log/alert)   │           │ (log/alert)   │
   └───────────────┘           └───────────────┘           └───────────────┘
```

```
Producer GO1  \
Producer GO2   \
Producer GO3    → UNIX Socket (/tmp/engine.sock)
Producer GO4   /
                   ↓
               EpollLoopThread
                   ↓
            ThreadSafeQueue<Event>
                   ↓
                  Worker
```

---

## Execution Flow

1. `Thread I/O` collecte un événement (CPU, memory, network, etc.)
2. Il pousse l'événement dans la `ThreadSafeQueue`
3. Un des workers disponibles pop l’événement
4. Le `RuleEngine` analyse l’événement
5. Si une règle match → déclenche une `Action`

---

## Worker Loop (conceptuel)

```
while (running)
{
    Event event = queue.wait_and_pop();
    ruleEngine.evaluate(event);
}
```

---

## Points Clés Architecture

- Queue thread-safe (mutex + condition_variable)
- Worker pool scalable
- Traitement parallèle
- Séparation I/O / Processing
- Extensible (nouvelles règles, nouvelles actions)

---